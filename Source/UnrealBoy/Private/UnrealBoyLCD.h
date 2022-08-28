// License: See LICENSE.txt file

#pragma once

#include "CoreMinimal.h"

class FUnrealBoyMotherboard;

class FUnrealBoyLCDCRegister
{
public:
	FUnrealBoyLCDCRegister(uint8& InLCDC);

	void SetValue(uint8 Value);
	
	bool IsLCDEnabled() const;
	bool GetWindowMapSelect() const;
	bool IsWindowEnabled() const;
	bool GetTileDataSelect() const;
	bool GetBackgroundMapSelect() const;
	bool GetSpriteHeight() const;
	bool IsSpriteEnabled() const;
	bool IsBackgroundEnabled() const;

private:
	uint8& LCDC;
};

class FUnrealBoyLCDStatRegister
{
public:
	FUnrealBoyLCDStatRegister(uint8& InStat);

	void SetValue(uint8 Value);
	uint8 UpdateLYC(uint8 LYC, uint8 LY) const;
	
	uint8 SetMode(uint8 InMode);
	uint8 GetMode() const;
private:
	uint8& Stat;
	uint8 Mode;
};

/**
 * Liquid-crystal display device.
 * We also simulate a picture processing unit (PPU) in it
 */
class FUnrealBoyLCD
{
public:
	FUnrealBoyLCD(FUnrealBoyMotherboard& InMotherboard);
	~FUnrealBoyLCD();

	/** Set the value of LCD control register */
	void SetLCDC(uint8 Value);

	/** Set the value of LCD status register */
	void SetStat(uint8 Value);

	/**
	 * Make one tick on the emulated LCD
	 * @return the interrupt flags to set
	 */
	uint8 Tick(uint32 Cycles);

	uint32 GetCyclesToInterrupt() const;

	bool ProcessingFrame();

	void AddChangedTile(uint16 Address);

	const TArray<FColor>& GetScreenBuffer() const;
	
	const TArray<FColor>& GetTileMap1Buffer() const;

	static uint32 NUM_ROWS;
	static uint32 NUM_COLS;
	static uint32 NUM_PIXELS;
	static uint32 NUM_TILES;
	static uint32 NUM_FRAME_CYCLES;
	static uint32 TILEMAP_WIDTH;
	static uint32 TILEMAP_HEIGHT;

	friend class FUnrealBoyPPU;
private:
	/** Reference to motherboard */
	FUnrealBoyMotherboard& Motherboard;

	/** LCD Control */
	FUnrealBoyLCDCRegister LCDC;
	
	/** LCD Status */
	FUnrealBoyLCDStatRegister LCDStat;
	
	/** Scroll Y */
	uint8& SCY;
	
	/** Scroll X */
	uint8& SCX;

	/** Y Coordinate of scan line */
	uint8& LY;

	/** LY Compare */
	uint8& LYC;
	
	uint8& BGP;
	uint8& OBP0;
	uint8& OBP1;

	/** Window Y position */
	uint8& WY;
	
	/** Window X position */
	uint8& WX;

	/** Video RAM */
	TArrayView<uint8> VRAM;

	/** Spirit attribute table */
	TArrayView<uint8> OAM;

	/** FColor palette */
	TArray<FColor> ColorPalette;

	/** All pixel colors in screen */
	TArray<FColor> ScreenBuffer;

	/** All pixel colors in tile map 1 */
	TArray<FColor> TileMap1Buffer;

	/** All pixel colors in tile map 2 */
	TArray<FColor> TileMap2Buffer;
	
	/** Picture process unit */
	TSharedPtr<class FUnrealBoyPPU> PPU;

	/** Status vars */
	uint8 NextStatMode;
	uint32 Clock;
	uint32 ClockTarget;
	bool bFrameDone;
	uint8 MaxLY;
	uint8 LYWindow;
	TSet<uint16> ChangedTileAddresses;

	/** Color caches */
	TArray<FColor> TileCache;
	TArray<FColor> SpriteCache0;
	TArray<FColor> SpriteCache1;

	TSet<uint16> TileCache_Debug;
};

class FUnrealBoyPPU
{
public:
	FUnrealBoyPPU(FUnrealBoyLCD& InLCD);

	void ScanLine();

	void ClearScreen();
protected:
	void UpdateCache();
	void ScanLineBGAndWindow();
	void ScanLineSprites();

	static uint8 CalcColorIndex(uint8 Byte1, uint8 Byte2, uint8 Offset);
	static uint8 GetColorCode(uint8 ColorPalette, uint8 ColorIndex);

private:
	FUnrealBoyLCD& LCD;
};