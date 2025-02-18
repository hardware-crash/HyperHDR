#pragma once

// STL includes
#include <cstdint>
#include <QString>
#include <QJsonObject>
#include <utils/Logger.h>

/// Correction for a single color byte value
/// All configuration values are unsigned int and assume the color value to be between 0 and 255
class RgbChannelAdjustment
{
public:
	/// Default constructor
	RgbChannelAdjustment(QString channelName = "");

	/// Constructor
	/// @param adjustR
	/// @param adjustG
	/// @param adjustB
	RgbChannelAdjustment(quint8 instance, uint8_t adjustR, uint8_t adjustG, uint8_t adjustB, QString channelName, bool enabled);

	///
	/// Transform the given array value
	///
	/// @param input The input color bytes
	/// @param brightness The current brightness value
	/// @param red The red color component
	/// @param green The green color component
	/// @param blue The blue color component
	///
	/// @note The values are updated in place.
	///
	void apply(uint8_t input, uint8_t brightness, uint8_t& red, uint8_t& green, uint8_t& blue);

	///
	/// setAdjustment RGB
	///
	/// @param adjustR
	/// @param adjustG
	/// @param adjustB
	///
	void setAdjustment(uint8_t adjustR, uint8_t adjustG, uint8_t adjustB);

	/// @return The current adjustR value
	uint8_t getAdjustmentR() const;

	/// @return The current adjustG value
	uint8_t getAdjustmentG() const;

	/// @return The current adjustB value
	uint8_t getAdjustmentB() const;

	uint8_t adjustmentR(uint8_t inputR) const;

	uint8_t adjustmentG(uint8_t inputG) const;

	uint8_t adjustmentB(uint8_t inputB) const;

	void setCorrection(uint8_t correction);

	uint8_t getCorrection() const;

	uint8_t correction(uint8_t input) const;

	bool isEnabled();

	static RgbChannelAdjustment createRgbChannelAdjustment(quint8 instance, const QJsonObject& colorConfig, const QString& channelName, int defaultR, int defaultG, int defaultB);

private:
	/// color channels
	enum ColorChannel { RED = 0, GREEN = 1, BLUE = 2 };

	/// reset init of color mapping
	void resetInitialized();

	/// (re)-initilize the color mapping
	void initializeAdjustMapping(uint8_t _adjustR, uint8_t _adjustG, uint8_t _adjustB);
	void initializeCorrectionMapping(uint8_t correction);

	int _mappingAdjR[256];
	int _mappingAdjG[256];
	int _mappingAdjB[256];

	/// The correction of R channel
	int _correction;

	/// The mapping from input color to output color
	int _mappingCorection[256];


	/// The adjustment of RGB channel
	uint8_t _adjust[3];

	/// The mapping from input color to output color
	uint8_t _mapping[3][256];

	/// Name of this channel, usefull for debug messages
	QString _channelName;

	/// Logger instance
	Logger* _log;

	/// bitfield to determine white value is alreade initialized
	bool _initialized[256];

	/// current brightness value
	uint8_t _brightness;

	bool _enabled;
};
