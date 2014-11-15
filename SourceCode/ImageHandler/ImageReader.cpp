#include <iostream>
#include <cstddef>
#include <jpeglib.h>
#include <ImageReader.h>
#include <stdexcept>
#include <cstring>

void readJpgImage(const std::string& imagePath, std::vector<ushort>& pixels,
                  unsigned int& width, unsigned int& height);

void writeJpgImage(const std::string& imagePath, const std::vector<ushort>& pixels,
                  unsigned int width, unsigned int height);

void readImage(const std::string& imagePath, const std::string& format,
               std::vector<ushort>& pixels, unsigned int& width,
               unsigned int& height)
{
    if (format == "jpg")
    {
        // clean pixels vector since must be fill with pixels from image
        pixels.clear();
        readJpgImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
    }
}

void writeImage(const std::string& imagePath, const std::string& format,
               const std::vector<ushort>& pixels, unsigned int width,
               unsigned int height)
{
    if (format == "jpg")
    {
        writeJpgImage(imagePath, pixels, width, height);
    }
    else
    {
        throw std::runtime_error("Unsupported format: " +format);
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

