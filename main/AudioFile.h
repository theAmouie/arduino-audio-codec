#include "LinkedList.h"
#include "Util.h"

/** The different types of audio file, plus some other types to 
 * indicate a failure to load a file, or that one hasn't been
 * loaded yet
 */
enum class AudioFileFormat
{
    Error,
    NotLoaded,
    Wave,
    Aiff
};


template <class T>
class AudioFile
{
public:
    
    // typedef std::vector<std::vector<T> > AudioBuffer;
    typedef LinkedList<LinkedList<T>> AudioBuffer;
    

    /** Constructor */
    AudioFile();
        

    /** Loads an audio file from a given file path.
     * @Returns true if the file was successfully loaded
     */
    // bool load (std::string filePath);
    bool load (String filePath);

    
    /** Saves an audio file to a given file path.
     * @Returns true if the file was successfully saved
     */
    // bool save (std::string filePath, AudioFileFormat format = AudioFileFormat::Wave);
    bool save (String filePath, AudioFileFormat format = AudioFileFormat::Wave);

        
    //=============================================================
    /** @Returns the sample rate */
    uint32_t getSampleRate() const;
    
    /** @Returns the number of audio channels in the buffer */
    int getNumChannels() const;
 
    /** @Returns true if the audio file is mono */
    bool isMono() const;
    
    /** @Returns true if the audio file is stereo */
    bool isStereo() const;
    
    /** @Returns the bit depth of each sample */
    int getBitDepth() const;
    
    /** @Returns the number of samples per channel */
    int getNumSamplesPerChannel() const;
    
    /** @Returns the length in seconds of the audio file based on the number of samples and sample rate */
    double getLengthInSeconds() const;
    
    /** Prints a summary of the audio file to the console */
    void printSummary() const;
    
    //=============================================================
    
    /** Set the audio buffer for this AudioFile by copying samples from another buffer.
     * @Returns true if the buffer was copied successfully.
     */
    bool setAudioBuffer (AudioBuffer& newBuffer);
    
    /** Sets the audio buffer to a given number of channels and number of samples per channel. This will try to preserve
     * the existing audio, adding zeros to any new channels or new samples in a given channel.
     */
    void setAudioBufferSize (int numChannels, int numSamples);
    
    /** Sets the number of samples per channel in the audio buffer. This will try to preserve
     * the existing audio, adding zeros to new samples in a given channel if the number of samples is increased.
     */
    void setNumSamplesPerChannel (int numSamples);
    
    /** Sets the number of channels. New channels will have the correct number of samples and be initialised to zero */
    void setNumChannels (int numChannels);
    
    /** Sets the bit depth for the audio file. If you use the save() function, this bit depth rate will be used */
    void setBitDepth (int numBitsPerSample);
    
    /** Sets the sample rate for the audio file. If you use the save() function, this sample rate will be used */
    void setSampleRate (uint32_t newSampleRate);
    
    //=============================================================
    /** A vector of vectors holding the audio samples for the AudioFile. You can 
     * access the samples by channel and then by sample index, i.e:
     *
     *      samples[channel][sampleIndex]
     */
    AudioBuffer samples;
    
private:
    
    //=============================================================
    enum class Endianness
    {
        LittleEndian,
        BigEndian
    };
    
    //=============================================================
    AudioFileFormat determineAudioFileFormat (String fileData);
    // bool decodeWaveFile (std::vector<uint8_t>& fileData);
    bool decodeWaveFile (String fileData);

    // bool decodeAiffFile (std::vector<uint8_t>& fileData);
    // bool decodeAiffFile (LinkedList<uint8_t>& fileData);
    
    //=============================================================
    // bool saveToWaveFile (std::string filePath);
    bool saveToWaveFile (String filePath);

    // bool saveToAiffFile (std::string filePath);
    // bool saveToAiffFile (String filePath);

    
    //=============================================================
    void clearAudioBuffer();
    
    //=============================================================
    // int32_t fourBytesToInt (std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int32_t fourBytesToInt (String source, int startIndex, Endianness endianness = Endianness::LittleEndian);

    // int16_t twoBytesToInt (std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int16_t twoBytesToInt (String source, int startIndex, Endianness endianness = Endianness::LittleEndian);

    // int getIndexOfString (std::vector<uint8_t>& source, std::string s);
    int getIndexOfString (String source, String s);

    
    //=============================================================
    T sixteenBitIntToSample (int16_t sample);
    int16_t sampleToSixteenBitInt (T sample);
    
    //=============================================================
    uint8_t sampleToSingleByte (T sample);
    T singleByteToSample (uint8_t sample);
    
    // bool tenByteMatch (std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2);
    // bool tenByteMatch (LinkedList<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2);

    T clamp (T v1, T minValue, T maxValue);
    
    //=============================================================
    // void addStringToFileData (std::vector<uint8_t>& fileData, std::string s);
    void addStringToFileData (LinkedList<uint8_t> fileData, String s);

    // void addInt32ToFileData (std::vector<uint8_t>& fileData, int32_t i, Endianness endianness = Endianness::LittleEndian);
    void addInt32ToFileData (LinkedList<uint8_t>& fileData, int32_t i, Endianness endianness = Endianness::LittleEndian);

    // void addInt16ToFileData (std::vector<uint8_t>& fileData, int16_t i, Endianness endianness = Endianness::LittleEndian);
    void addInt16ToFileData (LinkedList<uint8_t>& fileData, int16_t i, Endianness endianness = Endianness::LittleEndian);

    
    //=============================================================
    // bool writeDataToFile (std::vector<uint8_t>& fileData, std::string filePath);
    bool writeDataToFile (LinkedList<uint8_t>& fileData, String filePath);

    
    //=============================================================
    AudioFileFormat audioFileFormat;
    uint32_t sampleRate;
    int bitDepth;
};

//=============================================================
/* IMPLEMENTATION */
//=============================================================

//=============================================================
template <class T>
AudioFile<T>::AudioFile()
{
    bitDepth = 16;
    sampleRate = 44100;
    samples.resize(1);
    samples[0].resize(0);
    audioFileFormat = AudioFileFormat::NotLoaded;
}

//=============================================================
template <class T>
uint32_t AudioFile<T>::getSampleRate() const
{
    return sampleRate;
}

//=============================================================
template <class T>
int AudioFile<T>::getNumChannels() const
{
    return (int)samples.size();
}

//=============================================================
template <class T>
bool AudioFile<T>::isMono() const
{
    return getNumChannels() == 1;
}

//=============================================================
template <class T>
bool AudioFile<T>::isStereo() const
{
    return getNumChannels() == 2;
}

//=============================================================
template <class T>
int AudioFile<T>::getBitDepth() const
{
    return bitDepth;
}

//=============================================================
template <class T>
int AudioFile<T>::getNumSamplesPerChannel() const
{
    if (samples.size() > 0)
        return (int) samples[0].size();
    else
        return 0;
}

//=============================================================
template <class T>
double AudioFile<T>::getLengthInSeconds() const
{
    return (double)getNumSamplesPerChannel() / (double)sampleRate;
}

//=============================================================
template <class T>
void AudioFile<T>::printSummary() const
{
    // std::cout << "|======================================|" << std::endl;
    Serial.println("|======================================|");

    // std::cout << "Num Channels: " << getNumChannels() << std::endl;
    Serial.println(getNumChannels());

    // std::cout << "Num Samples Per Channel: " << getNumSamplesPerChannel() << std::endl;
    Serial.println(getNumSamplesPerChannel());

    // std::cout << "Sample Rate: " << sampleRate << std::endl;
    Serial.println(sampleRate);

    // std::cout << "Bit Depth: " << bitDepth << std::endl;
    Serial.println(bitDepth);

    // std::cout << "Length in Seconds: " << getLengthInSeconds() << std::endl;
    Serial.println(getLengthInSeconds());

    // std::cout << "|======================================|" << std::endl;
    Serial.println("|======================================|");

}

//=============================================================
template <class T>
bool AudioFile<T>::setAudioBuffer (AudioBuffer& newBuffer)
{
    int numChannels = (int)newBuffer.size();
    
    if (numChannels <= 0)
    {
        // assert (false && "The buffer your are trying to use has no channels");
        return false;
    }
    
    int numSamples = (int)newBuffer[0].size();
    
    // set the number of channels
    samples.resize (newBuffer.size());
    
    for (int k = 0; k < getNumChannels(); k++)
    {
        // assert (newBuffer[k].size() == numSamples);
        if (newBuffer[k].size() != numSamples)
            return false; // not sure whether this is handled completely
        
        samples[k].resize (numSamples);
        
        for (int i = 0; i < numSamples; i++)
        {
            samples[k][i] = newBuffer[k][i];
        }
    }
    
    return true;
}

//=============================================================
template <class T>
void AudioFile<T>::setAudioBufferSize (int numChannels, int numSamples)
{
    samples.resize (numChannels);
    setNumSamplesPerChannel (numSamples);
}

//=============================================================
template <class T>
void AudioFile<T>::setNumSamplesPerChannel (int numSamples)
{
    int originalSize = getNumSamplesPerChannel();
    
    for (int i = 0; i < getNumChannels();i++)
    {
        samples[i].resize (numSamples);
        
        // set any new samples to zero
        if (numSamples > originalSize)
            // std::fill (samples[i].begin() + originalSize, samples[i].end(), (T)0.);
            samples.fill(i + originalSize, samples.size()-1, (T) 0.); //not sure to whether use size-1 or size.

    }
}

//=============================================================
template <class T>
void AudioFile<T>::setNumChannels (int numChannels)
{
    int originalNumChannels = getNumChannels();
    int originalNumSamplesPerChannel = getNumSamplesPerChannel();
    
    samples.resize (numChannels);
    
    // make sure any new channels are set to the right size
    // and filled with zeros
    if (numChannels > originalNumChannels)
    {
        for (int i = originalNumChannels; i < numChannels; i++)
        {
            samples[i].resize (originalNumSamplesPerChannel);
            // std::fill (samples[i].begin(), samples[i].end(), (T)0.);
            samples.fill(i, samples.size()-1, (T) 0.); //not sure to whether use size-1 or size.

        }
    }
}

//=============================================================
template <class T>
void AudioFile<T>::setBitDepth (int numBitsPerSample)
{
    bitDepth = numBitsPerSample;
}

//=============================================================
template <class T>
void AudioFile<T>::setSampleRate (uint32_t newSampleRate)
{
    sampleRate = newSampleRate;
}

//=============================================================
template <class T>
bool AudioFile<T>::load (String fileData)
{
    //std::ifstream file (filePath, std::ios::binary);
    
    // check the file exists
    // if (! file.good())
    // {
        // std::cout << "ERROR: File doesn't exist or otherwise can't load file" << std::endl;
        // std::cout << filePath << std::endl;
        // return false;
    // }
    
    // file.unsetf (std::ios::skipws);
    // std::istream_iterator<uint8_t> begin (file), end; 
    // std::vector<uint8_t> fileData (begin, end);
    
    // get audio file format
    audioFileFormat = determineAudioFileFormat (fileData);
    
    if (audioFileFormat == AudioFileFormat::Wave)
    {
        return decodeWaveFile (fileData);
    }
    /*else if (audioFileFormat == AudioFileFormat::Aiff)
    {
        return decodeAiffFile (fileData);
    }*/
    else
    {
        // std::cout << "Audio File Type: " << "Error" << std::endl;
        Serial.println("Audio file type error");
        return false;
    }
}

//=============================================================
template <class T>
bool AudioFile<T>::decodeWaveFile (String fileData)
{
    // -----------------------------------------------------------
    // HEADER CHUNK
    // std::string headerChunkID (fileData.begin(), fileData.begin() + 4);
    String header = splitString(fileData, 0, 4);

    //int32_t fileSizeInBytes = fourBytesToInt (fileData, 4) + 8;
    // std::string format (fileData.begin() + 8, fileData.begin() + 12);
    String format = splitString(fileData, 8, 12);

    
    // -----------------------------------------------------------
    // try and find the start points of key chunks
    int indexOfDataChunk = getIndexOfString (fileData, "data");
    int indexOfFormatChunk = getIndexOfString (fileData, "fmt");
    
    // if we can't find the data or format chunks, or the IDs/formats don't seem to be as expected
    // then it is unlikely we'll able to read this file, so abort
    if (indexOfDataChunk == -1 || indexOfFormatChunk == -1 || headerChunkID != "RIFF" || format != "WAVE")
    {
        // std::cout << "ERROR: this doesn't seem to be a valid .WAV file" << std::endl;
        Serial.println("ERROR: this doesn't seem to be a valid .WAV file");

        return false;
    }
    
    // -----------------------------------------------------------
    // FORMAT CHUNK
    int f = indexOfFormatChunk;
    // String formatChunkID (fileData.begin() + f, fileData.begin() + f + 4);
    String formatChunkID = splitString(fileData, f, f + 4);

    //int32_t formatChunkSize = fourBytesToInt (fileData, f + 4);
    int16_t audioFormat = twoBytesToInt (fileData, f + 8);
    int16_t numChannels = twoBytesToInt (fileData, f + 10);
    sampleRate = (uint32_t) fourBytesToInt (fileData, f + 12);
    int32_t numBytesPerSecond = fourBytesToInt (fileData, f + 16);
    int16_t numBytesPerBlock = twoBytesToInt (fileData, f + 20);
    bitDepth = (int) twoBytesToInt (fileData, f + 22);
    
    int numBytesPerSample = bitDepth / 8;
    
    // check that the audio format is PCM
    if (audioFormat != 1)
    {
        // std::cout << "ERROR: this is a compressed .WAV file and this library does not support decoding them at present" << std::endl;
        Serial.println("ERROR: this is a compressed .WAV file and this library does not support decoding them at present");

        return false;
    }
    
    // check the number of channels is mono or stereo
    if (numChannels < 1 || numChannels > 2)
    {
        // std::cout << "ERROR: this WAV file seems to be neither mono nor stereo (perhaps multi-track, or corrupted?)" << std::endl;
        Serial.println("ERROR: this WAV file seems to be neither mono nor stereo (perhaps multi-track, or corrupted?)");

        return false;
    }
    
    // check header data is consistent
    if ((numBytesPerSecond != (numChannels * sampleRate * bitDepth) / 8) || (numBytesPerBlock != (numChannels * numBytesPerSample)))
    {
        // std::cout << "ERROR: the header data in this WAV file seems to be inconsistent" << std::endl;
        Serial.println("ERROR: the header data in this WAV file seems to be inconsistent");

        return false;
    }
    
    // check bit depth is either 8, 16 or 24 bit
    if (bitDepth != 8 && bitDepth != 16 && bitDepth != 24)
    {
        // std::cout << "ERROR: this file has a bit depth that is not 8, 16 or 24 bits" << std::endl;
        Serial.println("ERROR: this file has a bit depth that is not 8, 16 or 24 bits");

        return false;
    }
    
    // -----------------------------------------------------------
    // DATA CHUNK
    int d = indexOfDataChunk;
    String dataChunkID = splitString(fileData.begin() + d, fileData.begin() + d + 4);
    int32_t dataChunkSize = fourBytesToInt (fileData, d + 4);
    
    int numSamples = dataChunkSize / (numChannels * bitDepth / 8);
    int samplesStartIndex = indexOfDataChunk + 8;
    
    clearAudioBuffer();
    samples.resize (numChannels);
    
    for (int i = 0; i < numSamples; i++)
    {
        for (int channel = 0; channel < numChannels; channel++)
        {
            int sampleIndex = samplesStartIndex + (numBytesPerBlock * i) + channel * numBytesPerSample;
            
            if (bitDepth == 8)
            {
                T sample = singleByteToSample (fileData[sampleIndex]);
                samples[channel].Append(sample);
            }
            else if (bitDepth == 16)
            {
                int16_t sampleAsInt = twoBytesToInt (fileData, sampleIndex);
                T sample = sixteenBitIntToSample (sampleAsInt);
                samples[channel].Append(sample);
            }
            else if (bitDepth == 24)
            {
                int32_t sampleAsInt = 0;
                sampleAsInt = (fileData[sampleIndex + 2] << 16) | (fileData[sampleIndex + 1] << 8) | fileData[sampleIndex];
                
                if (sampleAsInt & 0x800000) //  if the 24th bit is set, this is a negative number in 24-bit world
                    sampleAsInt = sampleAsInt | ~0xFFFFFF; // so make sure sign is extended to the 32 bit float

                T sample = (T)sampleAsInt / (T)8388608.;
                samples[channel].Append(sample);
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

//=============================================================
/*template <class T>
bool AudioFile<T>::tenByteMatch (std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2)
{
    for (int i = 0; i < 10; i++)
    {
        if (v1[startIndex1 + i] != v2[startIndex2 + i])
            return false;
    }
    
    return true;
}*/

//=============================================================
template <class T>
bool AudioFile<T>::save (String filePath, AudioFileFormat format)
{
    if (format == AudioFileFormat::Wave)
    {
        return saveToWaveFile (filePath);
    }
    
    return false;
}

//=============================================================
template <class T>
bool AudioFile<T>::saveToWaveFile (String filePath)
{
    LinkedList<uint8_t> fileData;
    
    int32_t dataChunkSize = getNumSamplesPerChannel() * (getNumChannels() * bitDepth / 8);
    
    // -----------------------------------------------------------
    // HEADER CHUNK
    addStringToFileData (fileData, "RIFF");
    
    // The file size in bytes is the header chunk size (4, not counting RIFF and WAVE) + the format
    // chunk size (24) + the metadata part of the data chunk plus the actual data chunk size
    int32_t fileSizeInBytes = 4 + 24 + 8 + dataChunkSize;
    addInt32ToFileData (fileData, fileSizeInBytes);
    
    addStringToFileData (fileData, "WAVE");
    
    // -----------------------------------------------------------
    // FORMAT CHUNK
    addStringToFileData (fileData, "fmt ");
    addInt32ToFileData (fileData, 16); // format chunk size (16 for PCM)
    addInt16ToFileData (fileData, 1); // audio format = 1
    addInt16ToFileData (fileData, (uint16_t)getNumChannels()); // num channels
    addInt32ToFileData (fileData, (uint32_t)sampleRate); // sample rate
    
    int32_t numBytesPerSecond = (uint32_t) ((getNumChannels() * sampleRate * bitDepth) / 8);
    addInt32ToFileData (fileData, numBytesPerSecond);
    
    int16_t numBytesPerBlock = getNumChannels() * (bitDepth / 8);
    addInt16ToFileData (fileData, numBytesPerBlock);
    
    addInt16ToFileData (fileData, (uint16_t)bitDepth);
    
    // -----------------------------------------------------------
    // DATA CHUNK
    addStringToFileData (fileData, "data");
    addInt32ToFileData (fileData, dataChunkSize);
    
    for (int i = 0; i < getNumSamplesPerChannel(); i++)
    {
        for (int channel = 0; channel < getNumChannels(); channel++)
        {
            if (bitDepth == 8)
            {
                uint8_t byte = sampleToSingleByte (samples[channel][i]);
                fileData.push_back (byte);
            }
            else if (bitDepth == 16)
            {
                int16_t sampleAsInt = sampleToSixteenBitInt (samples[channel][i]);
                addInt16ToFileData (fileData, sampleAsInt);
            }
            else if (bitDepth == 24)
            {
                int32_t sampleAsIntAgain = (int32_t) (samples[channel][i] * (T)8388608.);
                
                uint8_t bytes[3];
                bytes[2] = (uint8_t) (sampleAsIntAgain >> 16) & 0xFF;
                bytes[1] = (uint8_t) (sampleAsIntAgain >>  8) & 0xFF;
                bytes[0] = (uint8_t) sampleAsIntAgain & 0xFF;
                
                fileData.Append(bytes[0]);
                fileData.Append(bytes[1]);
                fileData.Append(bytes[2]);
            }
            else
            {
                // assert (false && "Trying to write a file with unsupported bit depth");
                Serial.println("Trying to write a file with unsupported bit depth");
                return false;
            }
        }
    }
    
    // check that the various sizes we put in the metadata are correct
    if (fileSizeInBytes != (fileData.size() - 8) || dataChunkSize != (getNumSamplesPerChannel() * getNumChannels() * (bitDepth / 8)))
    {
        // std::cout << "ERROR: couldn't save file to " << filePath << std::endl;
        Serial.println("ERROR: couldn't save file to ", filePath);

        return false;
    }
    
    // try to write the file
    return writeDataToFile (fileData, filePath);
}


//=============================================================
template <class T>
bool AudioFile<T>::writeDataToFile (LinkedList<uint8_t>& fileData, String filePath)
{
    std::ofstream outputFile (filePath, std::ios::binary);
    
    if (outputFile.is_open())
    {
        for (int i = 0; i < fileData.size(); i++)
        {
            char value = (char) fileData[i];
            outputFile.write (&value, sizeof (char));
        }
        
        outputFile.close();
        
        return true;
    }
    
    return false;
}

//=============================================================
template <class T>
void AudioFile<T>::addStringToFileData (LinkedList<uint8_t>& fileData, String s)
{
    for (int i = 0; i < s.length();i++)
        fileData.Append((uint8_t) s[i]);
}

//=============================================================
template <class T>
void AudioFile<T>::addInt32ToFileData (LinkedList<uint8_t>& fileData, uint32_t i, Endianness endianness)
{
    uint8_t bytes[4];
    
    if (endianness == Endianness::LittleEndian)
    {
        bytes[3] = (i >> 24) & 0xFF;
        bytes[2] = (i >> 16) & 0xFF;
        bytes[1] = (i >> 8) & 0xFF;
        bytes[0] = i & 0xFF;
    }
    else
    {
        bytes[0] = (i >> 24) & 0xFF;
        bytes[1] = (i >> 16) & 0xFF;
        bytes[2] = (i >> 8) & 0xFF;
        bytes[3] = i & 0xFF;
    }
    
    for (int i = 0; i < 4; i++)
        fileData.Append(bytes[i]);
}

//=============================================================
template <class T>
void AudioFile<T>::addInt16ToFileData (LinkedList<uint8_t>& fileData, uint16_t i, Endianness endianness)
{
    uint8_t bytes[2];
    
    if (endianness == Endianness::LittleEndian)
    {
        bytes[1] = (i >> 8) & 0xFF;
        bytes[0] = i & 0xFF;
    }
    else
    {
        bytes[0] = (i >> 8) & 0xFF;
        bytes[1] = i & 0xFF;
    }
    
    fileData.Append(bytes[0]);
    fileData.Append(bytes[1]);
}

//=============================================================
template <class T>
void AudioFile<T>::clearAudioBuffer()
{
    for (int i = 0; i < samples.size();i++)
    {
        samples[i].clear();
    }
    
    samples.clear();
}

//=============================================================
template <class T>
AudioFileFormat AudioFile<T>::determineAudioFileFormat (String fileData)
{
    // std::string header (fileData.begin(), fileData.begin() + 4);
    String header = splitString(fileData, 0, 4);

    if (header == "RIFF")
        return AudioFileFormat::Wave;
    // else if (header == "FORM")
        // return AudioFileFormat::Aiff;
    else
        return AudioFileFormat::Error;
}

//=============================================================
template <class T>
int32_t AudioFile<T>::fourBytesToInt (String source, int startIndex, Endianness endianness)
{
    int32_t result;
    
    if (endianness == Endianness::LittleEndian)
        result = (source[startIndex + 3] << 24) | (source[startIndex + 2] << 16) | (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 24) | (source[startIndex + 1] << 16) | (source[startIndex + 2] << 8) | source[startIndex + 3];
    
    return result;
}

//=============================================================
template <class T>
int16_t AudioFile<T>::twoBytesToInt (String source, int startIndex, Endianness endianness)
{
    int16_t result;
    
    if (endianness == Endianness::LittleEndian)
        result = (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 8) | source[startIndex + 1];
    
    return result;
}

//=============================================================
template <class T>
int AudioFile<T>::getIndexOfString (String source, String stringToSearchFor)
{
    int index = -1;
    int stringLength = (int) stringToSearchFor.length();
    
    for (int i = 0; i < source.length() - stringLength;i++)
    {
        // std::string section (source.begin() + i, source.begin() + i + stringLength);
        String section = splitString(source, i, i + stringLength);
        
        if (section == stringToSearchFor)
        {
            index = i;
            break;
        }
    }
    
    return index;
}

//=============================================================
template <class T>
T AudioFile<T>::sixteenBitIntToSample (int16_t sample)
{
    return static_cast<T> (sample) / static_cast<T> (32768.);
}

//=============================================================
template <class T>
int16_t AudioFile<T>::sampleToSixteenBitInt (T sample)
{
    sample = clamp (sample, -1., 1.);
    return static_cast<int16_t> (sample * 32767.);
}

//=============================================================
template <class T>
uint8_t AudioFile<T>::sampleToSingleByte (T sample)
{
    sample = clamp (sample, -1., 1.);
    sample = (sample + 1.) / 2.;
    return static_cast<uint8_t> (sample * 255.);
}

//=============================================================
template <class T>
T AudioFile<T>::singleByteToSample (uint8_t sample)
{
    return static_cast<T> (sample - 128) / static_cast<T> (128.);
}

//=============================================================
template <class T>
T AudioFile<T>::clamp (T value, T minValue, T maxValue)
{
    // value = std::min (value, maxValue);
    value = ((value < minValue) ? value : minValue);

    // value = std::max (value, minValue);
    value = ((value > maxValue) ? value : maxValue);

    return value;
}

#endif /* AudioFile_h */
