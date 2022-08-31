// License: See LICENSE.txt file


#include "UnrealBoyLCD.h"

#include "UnrealBoyMotherboard.h"

uint32 FUnrealBoyLCD::NUM_ROWS = 144;
uint32 FUnrealBoyLCD::NUM_COLS = 160;
uint32 FUnrealBoyLCD::NUM_PIXELS = NUM_ROWS * NUM_COLS;
uint32 FUnrealBoyLCD::NUM_TILES = 384;
uint32 FUnrealBoyLCD::NUM_FRAME_CYCLES = 70224;
uint32 FUnrealBoyLCD::TILEMAP_WIDTH = 256;
uint32 FUnrealBoyLCD::TILEMAP_HEIGHT = 256;

FUnrealBoyLCD::FUnrealBoyLCD(FUnrealBoyMotherboard& InMotherboard)
	: Motherboard(InMotherboard)
	, LCDC(UNREALBOY_GET_MEMORY_REF(LCDCRegister))
	, LCDStat(UNREALBOY_GET_MEMORY_REF(LCDStatRegister))
	, SCY(UNREALBOY_GET_MEMORY_REF(LCD_SCY))
	, SCX(UNREALBOY_GET_MEMORY_REF(LCD_SCX))
	, LY(UNREALBOY_GET_MEMORY_REF(LCD_LY))
	, LYC(UNREALBOY_GET_MEMORY_REF(LCD_LYC))
	, BGP(UNREALBOY_GET_MEMORY_REF(LCD_BGP))
	, OBP0(UNREALBOY_GET_MEMORY_REF(LCD_OBP0))
	, OBP1(UNREALBOY_GET_MEMORY_REF(LCD_OBP1))
	, WY(UNREALBOY_GET_MEMORY_REF(LCD_WY))
	, WX(UNREALBOY_GET_MEMORY_REF(LCD_WX))
	, VRAM(UNREALBOY_GET_MEMORY_RANGE_VIEW(VRAM_BEGIN, VRAM_END))
	, OAM(UNREALBOY_GET_MEMORY_RANGE_VIEW(OAM_BEGIN, OAM_END))
	, NextStatMode(2)
	, Clock(0)
	, ClockTarget(0)
	, bFrameDone(false)
	, MaxLY(153)
	, LYWindow(-1)
{
	BGP = 0xFC;
	OBP0 = 0xFF;
	OBP1 = 0xFF;

	ColorPalette = {
		{0xFF, 0xFF, 0xFF, 0xFF}, // White
		{0x99, 0x99, 0x99, 0xFF}, // Dark gray
		{0x55, 0x55, 0x55, 0xFF}, // Light gray
		{0x00, 0x00, 0x00, 0xFF}, // Black
	};

	const uint32 NumPixels = FUnrealBoyLCD::NUM_PIXELS;
	ScreenBuffer.SetNumUninitialized(NumPixels);
	FMemory::Memset(ScreenBuffer.GetData(), 0xFF, sizeof(FColor) * ScreenBuffer.Num());

	const uint32 NumTileMapPixels = FUnrealBoyLCD::TILEMAP_WIDTH * FUnrealBoyLCD::TILEMAP_HEIGHT;
	TileMap1Buffer.SetNumUninitialized(NumTileMapPixels);
	FMemory::Memset(TileMap1Buffer.GetData(), 0xFF, sizeof(FColor) * TileMap1Buffer.Num());
	
	TileMap2Buffer.SetNumUninitialized(NumTileMapPixels);
	FMemory::Memset(TileMap2Buffer.GetData(), 0xFF, sizeof(FColor) * TileMap2Buffer.Num());

	const uint32 NumTilePixels = FUnrealBoyLCD::NUM_TILES * 8 * 8;
	TileCache.SetNumUninitialized(NumTilePixels);
	SpriteCache0.SetNumUninitialized(NumTilePixels);
	SpriteCache1.SetNumUninitialized(NumTilePixels);
	FMemory::Memset(TileCache.GetData(), 0xFF, sizeof(FColor) * TileCache.Num());
	FMemory::Memset(SpriteCache0.GetData(), 0xFF, sizeof(FColor) * SpriteCache0.Num());
	FMemory::Memset(SpriteCache1.GetData(), 0xFF, sizeof(FColor) * SpriteCache1.Num());
	
	PPU = MakeShareable(new FUnrealBoyPPU(*this));
	check(PPU);
}

FUnrealBoyLCD::~FUnrealBoyLCD()
{
}

void FUnrealBoyLCD::SetLCDC(uint8 Value)
{
	LCDC.SetValue(Value);
}

void FUnrealBoyLCD::SetStat(uint8 Value)
{
	LCDStat.SetValue(Value);
}

uint8 FUnrealBoyLCD::Tick(uint32 Cycles)
{
	uint8 InterruptFlags = 0;
	Clock += Cycles;

	if (LCDC.IsLCDEnabled())
	{
		if (Clock >= ClockTarget)
		{
			if (LY == MaxLY)
			{
				// Reset to new frame and start form mode 2
				NextStatMode = 2;
				LY = static_cast<uint8>(-1);
				Clock %= NUM_FRAME_CYCLES;
				ClockTarget %= NUM_FRAME_CYCLES;
			}

			// Change to next mode
			InterruptFlags |= LCDStat.SetMode(NextStatMode);

			if (LCDStat.GetMode() == 2) // Searching OAM
			{
				ClockTarget += 80;
				NextStatMode = 3;
				// Increment LY
				LY++;
				InterruptFlags |= LCDStat.UpdateLYC(LYC, LY);
			}
			else if (LCDStat.GetMode() == 3) // Transferring data to LCD Controller
			{
				ClockTarget += 170;
				NextStatMode = 0;
			}
			else if (LCDStat.GetMode() == 0) // HBlank
			{
				ClockTarget += 206;
				if (LY <= 143)
				{
					// Scan line one by one
					PPU->ScanLine();
					NextStatMode = 2;
				}
				else
				{
					NextStatMode = 1;
				}
			}
			else if (LCDStat.GetMode() == 1) // VBlank
			{
				ClockTarget += 456;
				NextStatMode = 1;
				if (LY == 144)
				{
					InterruptFlags |= UnrealBoy::INTR_VBLANK;
					bFrameDone = true;
				}

				// Increment LY
				LY++;
				InterruptFlags |= LCDStat.UpdateLYC(LYC, LY);
			}
		}
	}
	else
	{
		if (Clock >= NUM_FRAME_CYCLES)
		{
			bFrameDone = true;
			Clock %= NUM_FRAME_CYCLES;

			PPU->ClearScreen();
		}
	}

	return InterruptFlags;
}

uint32 FUnrealBoyLCD::GetCyclesToInterrupt() const
{
	return ClockTarget - Clock;
}

bool FUnrealBoyLCD::ProcessingFrame()
{
	const bool bKeepProcessing = !bFrameDone;
	if (bFrameDone)
	{
		OnFrameDone.Broadcast();
		// Clear vblank flag for next iteration
		bFrameDone = false;
	}
	return bKeepProcessing;
}

void FUnrealBoyLCD::AddChangedTile(uint16 Address)
{
	ChangedTileAddresses.Add(Address);
}

const TArray<FColor>& FUnrealBoyLCD::GetScreenBuffer() const
{
	return ScreenBuffer;
}

const TArray<FColor>& FUnrealBoyLCD::GetTileMap1Buffer() const
{
	return TileMap1Buffer;
}

void FUnrealBoyLCD::RequestClearCache()
{
	bNeedClearCache = true;
}

/*****************************************************************
 * LCDC Register
*****************************************************************/

FUnrealBoyLCDCRegister::FUnrealBoyLCDCRegister(uint8& InLCDC)
	: LCDC(InLCDC)
{
}

void FUnrealBoyLCDCRegister::SetValue(uint8 Value)
{
	LCDC = Value;
}

bool FUnrealBoyLCDCRegister::IsLCDEnabled() const { return LCDC & (1 << 7); }
bool FUnrealBoyLCDCRegister::GetWindowMapSelect() const { return LCDC & (1 << 6); }
bool FUnrealBoyLCDCRegister::IsWindowEnabled() const { return LCDC & (1 << 5); }
bool FUnrealBoyLCDCRegister::GetTileDataSelect() const { return LCDC & (1 << 4); }
bool FUnrealBoyLCDCRegister::GetBackgroundMapSelect() const { return LCDC & (1 << 3); }
bool FUnrealBoyLCDCRegister::GetSpriteHeight() const { return LCDC & (1 << 2); }
bool FUnrealBoyLCDCRegister::IsSpriteEnabled() const { return LCDC & (1 << 1); }
bool FUnrealBoyLCDCRegister::IsBackgroundEnabled() const { return LCDC & (1 << 0); }

/*****************************************************************
* LCD Stat Register
*****************************************************************/

FUnrealBoyLCDStatRegister::FUnrealBoyLCDStatRegister(uint8& InStat)
	: Stat(InStat)
	, Mode(0)
{
	Stat |= 80;	// Bit 7 is always set
}

void FUnrealBoyLCDStatRegister::SetValue(uint8 Value)
{
	// Bit 7 is always set, and bit 0-2 are read-only
	Value &= 0b01111000;
	// Preserve read-only bits and clear the rest
	Stat &= 0b10000111;
	Stat |= Value;
}

uint8 FUnrealBoyLCDStatRegister::UpdateLYC(uint8 LYC, uint8 LY) const
{
	if (LYC == LY)
	{
		// Sets the LYC flag
		Stat |= 0b100;
		// Check LYC interrupt enabled flag
		if (Stat & 0b01000000)
		{
			return UnrealBoy::INTR_LCDC;
		}
	}
	else
	{
		// Clear LYC flag
		Stat &= 0b11111011;
	}
	
	return 0;
}

uint8 FUnrealBoyLCDStatRegister::SetMode(uint8 InMode)
{
	if (Mode == InMode)
	{
		// Mode already set
		return 0;
	}

	Mode = InMode;
	// Clearing 2 LSB
	Stat &= 0b11111100;
	// Apply mode to LSB
	Stat |= Mode;

	// Check if interrupt is enabled for this mode
	// Mode "3" is not interruptable
	if (Mode != 3 && Stat & (1 << (Mode + 3)))
	{
		return UnrealBoy::INTR_LCDC;
	}
	return 0;
}

uint8 FUnrealBoyLCDStatRegister::GetMode() const
{
	return Mode;
}

/*****************************************************************
* Picture processing unit
*****************************************************************/

FUnrealBoyPPU::FUnrealBoyPPU(FUnrealBoyLCD& InLCD)
	: LCD(InLCD)
{

}

void FUnrealBoyPPU::ScanLine()
{
	UpdateCache();
	ScanLineBGAndWindow();
	ScanLineSprites();
}

void FUnrealBoyPPU::ClearScreen()
{
	FMemory::Memset(LCD.ScreenBuffer.GetData(), FMath::FRand() * 0xFF, sizeof(FColor) * LCD.ScreenBuffer.Num());
}

void FUnrealBoyPPU::UpdateCache()
{
	if (LCD.bNeedClearCache)
	{
		LCD.ChangedTileAddresses.Reset();
		for (uint16 Address = 0x8000; Address < 0x9800; Address += 16)
		{
			LCD.ChangedTileAddresses.Add(Address);
		}
		LCD.bNeedClearCache = false;
	}
	
	const uint32 NumCols = 8;
	for (const uint16 Address : LCD.ChangedTileAddresses)
	{
		for (uint8 Byte = 0; Byte < 16; Byte += 2) // 2 bytes for each line
		{
			const uint8 Byte1 = LCD.VRAM[Address + Byte - 0x8000];
			const uint8 Byte2 = LCD.VRAM[Address + Byte + 1 - 0x8000];
			const uint16 Y = (Address + Byte - 0x8000) / 2;

			for (uint8 X = 0; X < 8; X++)
			{
				const uint8 ColorIndex = CalcColorIndex(Byte1, Byte2, 7 - X);
				const uint32 CacheIndex = Y * NumCols + X;
				LCD.TileCache[CacheIndex] = LCD.ColorPalette[GetColorCode(LCD.BGP, ColorIndex)];
				LCD.SpriteCache0[CacheIndex] = LCD.ColorPalette[GetColorCode(LCD.OBP0, ColorIndex)];
				LCD.SpriteCache1[CacheIndex] = LCD.ColorPalette[GetColorCode(LCD.OBP1, ColorIndex)];

				// Transparent
				if (ColorIndex == 0)
				{
					LCD.SpriteCache0[CacheIndex].A = 0;
					LCD.SpriteCache1[CacheIndex].A = 0;
				}

				// Debug
				if (LCD.TileCache[CacheIndex].AlignmentDummy != 0xFFFFFFFF)
				{
					LCD.TileCache_Debug.Add(CacheIndex);
				}
			}
		}
	}

	LCD.ChangedTileAddresses.Reset();
}

void FUnrealBoyPPU::ScanLineBGAndWindow()
{
	uint8 BX = LCD.SCX; // Background X
	uint8 BY = LCD.SCY; // Background Y
	uint8 WX = LCD.WX - 7; // Window X
	uint8 WY = LCD.WY; // Window Y
	uint8 LY = LCD.LY; // Scan line Y coordinate

	uint16 BackgroundMapOffset = LCD.LCDC.GetBackgroundMapSelect() ? 0x1C00 : 0x1800;
	uint16 WindowsMapOffset = LCD.LCDC.GetWindowMapSelect() ? 0x1C00 : 0x1800;

	uint8 Offset = BX & 0b111;
	if (LCD.LCDC.IsWindowEnabled() && WY <= LY && WX < FUnrealBoyLCD::NUM_COLS)
	{
		LCD.LYWindow++;
	}

	for (uint8 X = 0; X < FUnrealBoyLCD::NUM_COLS; X++)
	{
		const uint32 ScreenBufferIndex = LY * FUnrealBoyLCD::NUM_COLS + X;
		if (LCD.LCDC.IsWindowEnabled() && WY <= LY && WX <= X)
		{
			const uint16 WindowAddress = WindowsMapOffset + LCD.LYWindow / 8 * 32 % 1024 + (X - WX) / 8 % 32;
			uint16 WindowTileIndex = LCD.VRAM[WindowAddress];
			// If using signed tile indices, modify index
			if (!LCD.LCDC.GetTileDataSelect())
			{
				// (x ^ 0x80 - 128) to convert to signed, then add 256 for offset (reduces to + 128)
				WindowTileIndex = (WindowTileIndex ^ 0x80) + 128;
			}
			
			const uint32 CacheIndex = (8 * WindowTileIndex + LCD.LYWindow % 8) * 8 + (X - WX) % 8;
			LCD.ScreenBuffer[ScreenBufferIndex] = LCD.TileCache[CacheIndex];
		}
		else if (LCD.LCDC.IsBackgroundEnabled())
		{
			const uint16 BackgroundAddress = BackgroundMapOffset + (LY + BY) / 8 * 32 % 1024 + (X + BX) / 8 % 32;
			uint16 BackgroundTileIndex = LCD.VRAM[BackgroundAddress];
			// If using signed tile indices, modify index
			if (!LCD.LCDC.GetTileDataSelect())
			{
				// (x ^ 0x80 - 128) to convert to signed, then add 256 for offset (reduces to + 128)
				BackgroundTileIndex = (BackgroundTileIndex ^ 0x80) + 128;
			}
			const uint32 CacheIndex = (8 * BackgroundTileIndex + (LY + BY) % 8) * 8 + (X + Offset) % 8;
			const FColor PixelColor = LCD.TileCache[CacheIndex];
			LCD.ScreenBuffer[ScreenBufferIndex] = PixelColor;
		}
		else
		{
			// If background is disabled, it becomes white
			LCD.ScreenBuffer[ScreenBufferIndex] = LCD.ColorPalette[0];
		}
	}

	if (LY == 143)
	{
		// Reset at the end of a frame. We set it to -1, so it will be 0 after the first increment
		LCD.LYWindow = -1;
	}
}

void FUnrealBoyPPU::ScanLineSprites()
{
	if (!LCD.LCDC.IsSpriteEnabled())
	{
		return;
	}

	const uint8 LY = LCD.LY;
	const uint8 SpriteHeight = LCD.LCDC.GetSpriteHeight() ? 16 : 8;

	uint8 SpriteCount = 0;
	TArray<uint8> SpritesToRenderN;
	TArray<uint8> SpritesToRenderX;
	SpritesToRenderN.SetNumZeroed(10);
	SpritesToRenderX.SetNumZeroed(10);

	// Find the first 10 sprites in OAM that appears on this scanline.
	// The lowest X-coordinate has priority, when overlapping
	for (uint8 N = 0; N < 0xA0; N += 4)
	{
		// Documentation states the y coordinate needs to be subtracted by 16
		const uint8 Y = LCD.OAM[N] - 16;
		// Documentation states the x coordinate needs to be subtracted by 8
		const uint8 X = LCD.OAM[N + 1] - 8;

		if (Y <= LY && LY < Y + SpriteHeight)
		{
			SpritesToRenderN[SpriteCount] = N;
			// Used for sorting for priority
			SpritesToRenderX[SpriteCount] = X;
			SpriteCount++;
		}

		if (SpriteCount == 10)
		{
			break;
		}
	}

	if (SpriteCount == 0)
	{
		return;
	}

	// Pan docs:
	// When these 10 sprites overlap, the highest priority one will appear above all others, etc. (Thus, no
	// Z-fighting.) In CGB mode, the first sprite in OAM ($FE00-$FE03) has the highest priority, and so on. In
	// Non-CGB mode, the smaller the X coordinate, the higher the priority. The tie breaker (same X coordinates) is
	// the same priority as in CGB mode.
	TArray<uint8> SpritePriorities;
	SpritePriorities.SetNumUninitialized(SpriteCount);
	for (uint8 SpriteIndex = 0; SpriteIndex < SpriteCount; SpriteIndex++)
	{
		SpritePriorities[SpriteIndex] = SpriteIndex;
	}
	SpritePriorities.Sort([&SpritesToRenderN, SpritesToRenderX](uint8 LeftIndex, uint8 RightIndex)
	{
		if (SpritesToRenderX[LeftIndex] != SpritesToRenderX[RightIndex])
		{
			return SpritesToRenderX[LeftIndex] > SpritesToRenderX[RightIndex];
		}
		return SpritesToRenderN[LeftIndex] > SpritesToRenderN[RightIndex];
	});

	FColor BGPClearColor = LCD.ColorPalette[GetColorCode(LCD.BGP, 0)];
	for (const uint8 NIndex : SpritePriorities)
	{
		uint8 N = SpritesToRenderN[NIndex];
		// Documentation states the y coordinate needs to be subtracted by 16
		const uint8 Y = LCD.OAM[N] - 16;
		// Documentation states the x coordinate needs to be subtracted by 8
		uint8 X = LCD.OAM[N + 1] - 8;
		uint8 TileIndex = LCD.OAM[N + 2];
		if (SpriteHeight == 16)
		{
			// There two byte indexes
			TileIndex &= 0b11111110;
		}
		uint8 Attribute = LCD.OAM[N + 3];
		const bool bXFlip = Attribute & 0b00100000;
		const bool bYFlip = Attribute & 0b01000000;
		const bool bEnableBGOverObj = Attribute & 0b10000000;
		TArray<FColor>& SpriteCache = Attribute & 0b10000 ? LCD.SpriteCache1 : LCD.SpriteCache0;

		const uint8 DY = LY - Y;
		const uint8 YY = bYFlip ? SpriteHeight - DY - 1 : DY;
		for (uint8 DX = 0; DX < 8; DX++)
		{
			const uint32 ScreenBufferIndex = LY * FUnrealBoyLCD::NUM_COLS + X;
			const uint16 XX = bXFlip ? 7 - DX : DX;
			FColor PixelColor = SpriteCache[(8 * TileIndex + YY) * 8 + XX];
			if (X < FUnrealBoyLCD::NUM_COLS)
			{
				// TODO: Checking `LCD.ScreenBuffer[ScreenBufferIndex] != BGPClearColor` is a bit of a hack
				if (bEnableBGOverObj && LCD.ScreenBuffer[ScreenBufferIndex] != BGPClearColor)
				{
					PixelColor.A = 0;
				}
				
				if (PixelColor.A)
				{
					LCD.ScreenBuffer[ScreenBufferIndex] = PixelColor;
				}
			}
			X++;
		}
	}
}

uint8 FUnrealBoyPPU::CalcColorIndex(uint8 Byte1, uint8 Byte2, uint8 Offset)
{
	return (((Byte2 >> (Offset)) & 0b1) << 1) + ((Byte1 >> (Offset)) & 0b1);
}

uint8 FUnrealBoyPPU::GetColorCode(uint8 ColorPalette, uint8 ColorIndex)
{
	return (ColorPalette >> (ColorIndex * 2)) & 0b11;
}
