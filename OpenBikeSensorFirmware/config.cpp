/*
  Copyright (C) 2019 Zweirat
  Contact: https://openbikesensor.org

  This file is part of the OpenBikeSensor project.

  The OpenBikeSensor sensor firmware is free software: you can redistribute
  it and/or modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  The OpenBikeSensor sensor firmware is distributed in the hope that it will
  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
  Public License for more details.

  You should have received a copy of the GNU General Public License along with
  the OpenBikeSensor sensor firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

void loadConfiguration(const char *configFilename, Config &config) {
  // Open file for reading
  File file = SPIFFS.open(configFilename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<4096> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  config.numSensors = doc["numSensors"] | 2;
  for (size_t idx = 0; idx < config.numSensors; ++idx)
  {
    uint8_t offsetTemp;
    String offsetString = "offsetInfo" + String(idx);
    offsetTemp = doc[offsetString] | 35;
    config.sensorOffsets.push_back(offsetTemp);
  }

  strlcpy(config.ssid, doc["ssid"] | "Freifunk", sizeof(config.ssid));
  strlcpy(config.password, doc["password"] | "Freifunk", sizeof(config.password));
  strlcpy(config.obsUserID, doc["obsUserID"] | "5e8f2f43e7e3b3668ca13151", sizeof(config.obsUserID));
  config.displayConfig = doc["displayConfig"] | DisplaySimple;
  config.GPSConfig = doc["GPSConfig"] | NumberSatellites;
  config.port = doc["port"] | 2731;
  strlcpy(config.hostname,                              // <- destination
          doc["hostname"] | "openbikesensor.hlrs.de",   // <- source
          sizeof(config.hostname)                       // <- destination's capacity
  );
  config.satsForFix = doc["satsForFix"] | 4;
  config.confirmationTimeWindow = doc["confirmationTimeWindow"] | 5;
  config.privacyConfig = doc["privacyConfig"] | AbsolutePrivacy;

  config.numPrivacyAreas = doc["numPrivacyAreas"] | 0;
  for (size_t idx = 0; idx < config.numPrivacyAreas; ++idx)
  {
    PrivacyArea pricacyAreaTemp;
    // Original coordinates
    String latitudeString = "privacyLatitude" + String(idx);
    pricacyAreaTemp.latitude = doc[latitudeString] | 51.0;
    String longitudeString = "privacyLongitude" + String(idx);
    pricacyAreaTemp.longitude = doc[longitudeString] | 9.0;

    // randomly transformed coordinates
    String transformedLatitudeString = "privacyTransformedLatitude" + String(idx);
    pricacyAreaTemp.transformedLatitude = doc[transformedLatitudeString] | 51.0;
    String transformedLongitudeString = "privacyTransformedLongitude" + String(idx);
    pricacyAreaTemp.transformedLongitude = doc[transformedLongitudeString] | 9.0;

    // Radius
    String radiusString = "privacyRadius" + String(idx);
    pricacyAreaTemp.radius = doc[radiusString] | 500.0;
    config.privacyAreas.push_back(pricacyAreaTemp);
  }

  // Fix invalid "old" broken configurations, where the default value was 0
  if(config.privacyConfig == 0) config.privacyConfig = AbsolutePrivacy;
  if(config.displayConfig == 0) config.displayConfig = DisplaySimple;
  if(config.GPSConfig == 0) config.GPSConfig = NumberSatellites;

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<4096> doc;

  // Set the values in the document
  doc["numSensors"] = config.numSensors;
  for (size_t idx = 0; idx < config.numSensors; ++idx)
  {
    String offsetString = "offsetInfo" + String(idx);
    doc[offsetString] = config.sensorOffsets[idx];
  }
  doc["satsForFix"] = config.satsForFix;

  doc["hostname"] = config.hostname;
  doc["port"] = config.port;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["obsUserID"] = config.obsUserID;
  doc["displayConfig"] = config.displayConfig;
  doc["GPSConfig"] = config.GPSConfig;
  doc["confirmationTimeWindow"] = config.confirmationTimeWindow;
  doc["privacyConfig"] = config.privacyConfig;

  doc["numPrivacyAreas"] = config.numPrivacyAreas;
  for (size_t idx = 0; idx < config.numPrivacyAreas; ++idx)
  {
    //String latitudeString = "privacyLatitude" + String(idx);
    //JsonArray data = doc.createNestedArray(latitudeString);

    String latitudeString = "privacyLatitude" + String(idx);
    doc[latitudeString] = config.privacyAreas[idx].latitude;
    String longitudeString = "privacyLongitude" + String(idx);
    doc[longitudeString] = config.privacyAreas[idx].longitude;

    String transformdedLatitudeString = "privacyTransformedLatitude" + String(idx);
    doc[transformdedLatitudeString] = config.privacyAreas[idx].transformedLatitude;
    String transformdedLongitudeString = "privacyTransformedLongitude" + String(idx);
    doc[transformdedLongitudeString] = config.privacyAreas[idx].transformedLongitude;

    String radiusString = "privacyRadius" + String(idx);
    doc[radiusString] = config.privacyAreas[idx].radius;
  }

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printConfig(Config &config) {

  Serial.println(F("################################"));
  Serial.print(F("numSensors = "));
  Serial.println(String(config.numSensors));

  for (size_t idx = 0; idx < config.numSensors; ++idx)
  {
    String offsetString = "Offset[" + String(idx) + "] = " + config.sensorOffsets[idx];
    Serial.println(offsetString);
  }

  Serial.print(F("satsForFix = "));
  Serial.println(String(config.satsForFix));

  Serial.print(F("displayConfig = "));
  Serial.println(String(config.displayConfig));

  Serial.print(F("GPSConfig = "));
  Serial.println(String(config.GPSConfig));

  Serial.print(F("confirmationTimeWindow = "));
  Serial.println(String(config.confirmationTimeWindow));

  Serial.print(F("hostname = "));
  Serial.println(String(config.hostname));

  Serial.print(F("port = "));
  Serial.println(String(config.port));

  Serial.print(F("obsUserID = "));
  Serial.println(String(config.obsUserID));

  Serial.print(F("SSID = "));
  Serial.println(String(config.ssid));

  Serial.print(F("numPrivacyAreas = "));
  Serial.println(String(config.numPrivacyAreas));

  for (size_t idx = 0; idx < config.numPrivacyAreas; ++idx)
  {
    String latitudeString = "privacyLatitude[" + String(idx) + "] = " + String(config.privacyAreas[idx].latitude, 7);
    Serial.println(latitudeString);

    String longitudeString = "privacyLongitude[" + String(idx) + "] = " + String(config.privacyAreas[idx].longitude, 7);
    Serial.println(longitudeString);

    String transformdedLatitudeString = "privacyTransformedLatitude[" + String(idx) + "] = " + String(config.privacyAreas[idx].transformedLatitude, 7);
    Serial.println(transformdedLatitudeString);

    String transformdedLongitudeString = "privacyTransformedLongitude[" + String(idx) + "] = " + String(config.privacyAreas[idx].transformedLongitude, 7);
    Serial.println(transformdedLongitudeString);

    String radiusString = "privacyRadius[" + String(idx) + "] = " + config.privacyAreas[idx].radius;
    Serial.println(radiusString);
  }

#ifdef dev
  Serial.print(F("password = "));
  Serial.println(String(config.password));
#endif

  Serial.println(F("################################"));

  /*
    doc["displayConfig"] = config.displayConfig;

  */



  /*
    // Open file for reading
    File file = SPIFFS.open(configFilename);
    if (!file) {
    Serial.println(F("Failed to read file"));
    return;
    }

    // Extract each characters by one by one
    while (file.available()) {
    // do not ptrint passwords, please it is bad enough that we save them as plain text anyway...
    Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file
    file.close();
  */
}
