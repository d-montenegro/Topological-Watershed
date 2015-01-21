#include <iostream>
#include <cstddef>
#include <jpeglib.h>
#include <ImageReader.h>
#include <stdexcept>
#include <cstring>
#include <png++/image.hpp>

void readJpgImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width, unsigned int& height);

void readPngImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width, unsigned int& height);

void writeJpgImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                  unsigned int width, unsigned int height);

void writePngImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                  unsigned int width, unsigned int height);

void readPGMImage(const string& imgPath, vector<ushort>& image,
        unsigned int& width, unsigned int& height, unsigned int& maxValue);

void writePGMImage(const string& imgPath, const vector<ushort>& image,
        unsigned int width, unsigned int height, unsigned int maxValue);

void readImage(const std::string& imagePath, IMAGE_FORMAT format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height)
{
    if (format == JPG || format == JPEG)
    {
        // clean pixels vector since must be fill with pixels from image
        pixels.clear();
        readJpgImage(imagePath, pixels, width, height);
    }
    else if (format == PNG)
    {
        pixels.clear();
        readPngImage(imagePath, pixels, width, height);
    }
    else if(format == PGM)
    {
        pixels.clear();
        unsigned int maxValue = 0;
        readPGMImage(imagePath,pixels,width,height,maxValue);
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
    }
}

void writeImage(const std::string& imagePath, IMAGE_FORMAT format,
               const std::vector<ushort>& pixels, unsigned int width,
               unsigned int height)
{
    if (format == JPG || format == JPEG)
    {
        writeJpgImage(imagePath, pixels, width, height);
    }
    else if (format == PNG)
    {
        writePngImage(imagePath, pixels, width, height);
    }
    else if(format == PGM)
    {
        writePGMImage(imagePath,pixels,width,height,255); // reveer lo del 255
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
    }
}

void readPngImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width,
                  unsigned int& height)
{
    png::image<png::gray_pixel> image(imagePath);
    width = image.get_width();
    height = image.get_height();

    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            pixels.push_back(image.get_pixel(x,y));
        }
    }
}

void readJpgImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width,
                  unsigned int& height)
{
    FILE * infile = 0;
    if ((infile = fopen(imagePath.c_str(), "rb")) == NULL)
    {
        throw std::invalid_argument("Could not open file " +imagePath);
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    if (JCS_GRAYSCALE != cinfo.jpeg_color_space)
    {
        throw std::invalid_argument("The image " +imagePath+ " is not greyscale.");
    }

    width = cinfo.output_width;
    height = cinfo.output_height;

    JSAMPARRAY pJpegBuffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, width, 1);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        for (unsigned int x = 0; x < width; x++)
        {
            pixels.push_back(pJpegBuffer[0][x]);
        }
    }

    fclose(infile);
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}

void writeJpgImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                   unsigned int width, unsigned int height)
{
    FILE * outfile = 0;
    if ((outfile = fopen(imagePath.c_str(), "wb")) == 0) {
        throw std::invalid_argument("Could not open file " +imagePath+ " for write.");
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width  = width;
    cinfo.image_height = height;
    cinfo.input_components = 1;
    cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,1,true);
    jpeg_set_linear_quality(&cinfo,1,true);

    unsigned char* image_buffer = (unsigned char*) malloc(width * height);

    unsigned int i = 0;
    for(auto& pixel : pixels)
    {
        image_buffer[i] = pixel;
        i++;
    }

    JSAMPROW row_pointer = 0;
    jpeg_start_compress(&cinfo, true);
    while (cinfo.next_scanline < height) {
        row_pointer = &image_buffer[cinfo.next_scanline * width];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    free(image_buffer);
}

void writePngImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                   unsigned int width, unsigned int height)
{
    png::image<png::gray_pixel> image(width,height);

    for(unsigned int y = 0; y < height; y++)
    {
        for(unsigned int x = 0; x < width; x++)
        {
            image.set_pixel(x,y,pixels.at(x + y * width));
        }
    }

    try
    {
        image.write(imagePath);
    }
    catch (runtime_error e)
    {
        throw runtime_error("Could not write on: \"" + imagePath +
                            "\". Reason: " + string(e.what()));
    }
}

void readPGMImage(const string& imgPath, vector<ushort>& image,
        unsigned int& width, unsigned int& height, unsigned int& maxValue)
{
    image.clear();
    ifstream pgmImage(imgPath.c_str(),ios::binary);
    if (pgmImage.is_open())
    {
        // get image type (P5 means binary and P2 means ASCII)
        string type;
        pgmImage >> type;
        if(type != "P2" && type != "P5")
        {
            throw invalid_argument("Only type P2 and P5 are supported");
        }

        // read width
        pgmImage >> width;

        // read height
        pgmImage >> height;

        // read maxValue
        pgmImage >> maxValue;

        if(maxValue != 255)
        {
            throw invalid_argument("Only 255 grey values are supported");
        }

        if(type == "P5")
        {
            // it's binary type
            unsigned char c = ' ';
            while(pgmImage >> c)
            {
                image.push_back(c);
            }
        }
        else
        {
            // it's plain type
            unsigned int n = 0;
            while (pgmImage >> n)
            {
                image.push_back(n);
            }
        }

        pgmImage.close();
    }
    else
    {
        throw invalid_argument("Could not read " + imgPath);
    }
}

void writePGMImage(const string& imgPath, const vector<ushort>& image,
        unsigned int width, unsigned int height, unsigned int maxValue)
{
    ofstream pgmImage;
    pgmImage.open(imgPath.c_str());
    if (pgmImage.is_open())
    {
        pgmImage << "P2" << endl;
        pgmImage << width << " " << height << endl;
        pgmImage << maxValue << endl;
        for(unsigned int i = 0; i < image.size() - 1; i++)
        {
            if(i != 0 && i % width == 0)
            {
                pgmImage << endl;
            }
            pgmImage << image.at(i) << " ";
        }
        pgmImage << image.at(image.size() - 1) << endl;
        pgmImage.close();
    }
    else
    {
        throw invalid_argument("Could not write " + imgPath);
    }
}
