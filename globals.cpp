#include "Utilities/globals.hpp"

namespace globals {
fs::path inputFile;
fs::path outputFile;
bitfake::type::AudioFormat outputFormat = bitfake::type::AudioFormat::MP3;
bitfake::type::VBRQualities VBRQuality;
int opusBitrateKbps = 192; // default 192
bool outputToTerminal = true;
std::string version = "2.0";
fs::path conversionOutputDirectory;
std::string tag, val;
} // namespace globals
