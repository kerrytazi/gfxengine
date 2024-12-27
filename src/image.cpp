#include "gfxengine/image.hpp"

#include "png.h"

#include <string>
#include <cstdio>

Image Image::load_sync(std::string_view file_name)
{
	FILE *f = fopen(std::string(file_name).c_str(), "rb");

	if (!f)
		throw 1;

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<uint8_t> data(size);
	fread(data.data(), 1, size, f);

	return load(data);
}

Image Image::load(std::span<const uint8_t> file_data)
{
	if (png_sig_cmp(file_data.data(), 0, 8) != 0)
		throw 1;

	// create main struct and install the custom error handler
	png_struct *m_pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

	if (m_pPngStruct == nullptr)
	{
		throw 1;
	}

	png_info *m_pPngInfo = png_create_info_struct(m_pPngStruct);

	if (m_pPngInfo == nullptr)
	{
		png_destroy_read_struct( &m_pPngStruct, nullptr, nullptr );
		m_pPngStruct = nullptr;
		throw 1;
	}

	// create info struct for post-IDAT chunks
	png_info *m_pPngEndInfo = png_create_info_struct(m_pPngStruct);

	if (m_pPngEndInfo == nullptr)
	{
		png_destroy_read_struct( &m_pPngStruct, &m_pPngInfo, nullptr );
		m_pPngStruct = nullptr;
		m_pPngInfo = nullptr;
		throw 1;
	}

	struct MemoryFile
	{
		std::span<const uint8_t> data;
		size_t count = 0;

		void read(uint8_t *out, size_t size)
		{
			for (size_t i = 0; i < size; ++i)
				out[i] = data[count + i];

			count += size;
		}
	};

	MemoryFile ff{ file_data };

	const png_rw_ptr ReadDataFromInputStream = [](png_struct *png_ptr, png_byte *out, size_t count) {
		MemoryFile *ff = (MemoryFile *)png_get_io_ptr(png_ptr);
		ff->read(out, count);
	};

	// set the struct to read the file
	// png_init_io(m_pPngStruct, m_pPngFile);
	png_set_read_fn(m_pPngStruct, &ff, ReadDataFromInputStream);

	// 8 bytes already read in OpenFile()
	// png_set_sig_bytes(m_pPngStruct, 8);

	// optional: keep all unknown chunks
	// png_set_keep_unknown_chunks(m_pPngStruct, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0 );

	png_read_info(m_pPngStruct, m_pPngInfo);   // read prior-IDAT chunks

	size_t RowLen = png_get_rowbytes(m_pPngStruct, m_pPngInfo);
	size_t Height = png_get_image_height(m_pPngStruct, m_pPngInfo);

	Image result;
	result.data.resize(Height * RowLen);
	result.width = RowLen / 4;
	result.height = Height;
	result.format = Format::RGBA;

	auto RowPtrs = std::make_unique<uint8_t*[]>(Height);
	for (size_t i = 0; i < Height; ++i)
		RowPtrs[i] = &result.data[i * RowLen];

	png_read_image(m_pPngStruct, RowPtrs.get());

	png_read_end(m_pPngStruct, m_pPngEndInfo); // read post-IDAT chunks

	return result;
}
