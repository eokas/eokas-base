
#include "stream.h"
#include "string.h"

_BeginNamespace(eokas)

DataStream::DataStream(Stream* stream)
	:mBaseStream(stream)
{}

DataStream::~DataStream()
{}

/*
============================================================================================
==== Override
============================================================================================
*/

bool DataStream::open()
{
	return mBaseStream->open();
}

void DataStream::close()
{
	return mBaseStream->close();
}

bool DataStream::isOpen() const
{
	return mBaseStream->isOpen();
}

bool DataStream::readable() const
{
	return mBaseStream->readable();
}

bool DataStream::writable() const
{
	return mBaseStream->writable();
}

bool DataStream::eos() const
{
	return mBaseStream->eos();
}

size_t DataStream::pos() const
{
	return mBaseStream->pos();
}

size_t DataStream::size() const
{
	return mBaseStream->size();
}

size_t DataStream::read(void* data, size_t size)
{
	return mBaseStream->read(data, size);
}

size_t DataStream::write(void* data, size_t size)
{
	return mBaseStream->write(data, size);
}

bool DataStream::seek(int offset, int origin) // 0:beg, 1:cur, 2:end
{
	return mBaseStream->seek(offset, origin);
}

void DataStream::flush()
{
	return mBaseStream->flush();
}

/*
============================================================================================
==== Typed value reader and writer
============================================================================================
*/
template<typename T>
inline bool ReadValue(DataStream& stream, T& value)
{
	size_t size = sizeof(T);
	size_t rlen = stream.read((void*)&value, size);
	return rlen == size;
}
template<>
inline bool ReadValue<String>(DataStream& stream, String& value)
{
	u16_t size = 0;
	if(!ReadValue(stream, size))
		return false;
	value = String('\0', (size_t)size);
	size_t rlen = stream.read((void*)value.cstr(), size);
	return rlen == size;
}

template <typename T>
inline bool WriteValue(DataStream& stream, const T& value)
{
	size_t size = sizeof(T);
	size_t wlen = stream.write((void*)&value, size);
	return wlen == size;
}
template <>
inline bool WriteValue<String>(DataStream& stream, const String& value)
{
	u16_t size = (u16_t)value.length();
	if(!WriteValue(stream, size))
		return false;
	size_t wlen = stream.write((void*)value.cstr(), size);
	return wlen == size;
}

template<typename T>
inline T ReadWithDefaultValue(DataStream& stream, const T& defaultValue)
{
    T value = defaultValue;
    if(!ReadValue(stream, value))
        return defaultValue;
    return value;
}
//============================================================
i8_t DataStream::readI8(const i8_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u8_t DataStream::readU8(const u8_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i16_t DataStream::readI16(const i16_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u16_t DataStream::readU16(const u16_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i32_t DataStream::readI32(const i32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u32_t DataStream::readU32(const u32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i64_t DataStream::readI64(const i64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u64_t DataStream::readU64(const u64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
f32_t DataStream::readF32(const f32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
f64_t DataStream::readF64(const f64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
bool DataStream::readBool(const bool& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
String DataStream::readString(const String& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
//============================================================
bool DataStream::read(i8_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(u8_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(i16_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(u16_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(i32_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(u32_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(i64_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(u64_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(f32_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(f64_t& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(bool& value)
{
	return ReadValue(*this, value);
}
bool DataStream::read(String& value)
{
	return ReadValue(*this, value);
}
//=============================================================
bool DataStream::write(i8_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(u8_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(i16_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(u16_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(i32_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(u32_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(i64_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(u64_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(f32_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(f64_t value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(bool value)
{
	return WriteValue(*this, value);
}
bool DataStream::write(const String& value)
{
	return WriteValue(*this, value);
}
//-----------------------------------------------------------------
_EndNamespace(eokas)
