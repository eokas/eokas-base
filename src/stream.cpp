
#include "stream.h"
#include "string.h"

_BeginNamespace(eokas)

template<typename T>
inline bool ReadValue(Stream& stream, T& value)
{
	size_t size = sizeof(T);
	size_t rlen = stream.read((void*)&value, size);
	return rlen == size;
}
template<>
inline bool ReadValue<String>(Stream& stream, String& value)
{
	u16_t size = 0;
	if(!ReadValue(stream, size))
		return false;
	value = String('\0', (size_t)size);
	size_t rlen = stream.read((void*)value.cstr(), size);
	return rlen == size;
}

template <typename T>
inline bool WriteValue(Stream& stream, const T& value)
{
	size_t size = sizeof(T);
	size_t wlen = stream.write((void*)&value, size);
	return wlen == size;
}
template <>
inline bool WriteValue<String>(Stream& stream, const String& value)
{
	u16_t size = (u16_t)value.length();
	if(!WriteValue(stream, size))
		return false;
	size_t wlen = stream.write((void*)value.cstr(), size);
	return wlen == size;
}

template<typename T>
inline T ReadWithDefaultValue(Stream& stream, const T& defaultValue)
{
    T value = defaultValue;
    if(!ReadValue(stream, value))
        return defaultValue;
    return value;
}
//============================================================
i8_t Stream::readI8(const i8_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u8_t Stream::readU8(const u8_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i16_t Stream::readI16(const i16_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u16_t Stream::readU16(const u16_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i32_t Stream::readI32(const i32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u32_t Stream::readU32(const u32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
i64_t Stream::readI64(const i64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
u64_t Stream::readU64(const u64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
f32_t Stream::readF32(const f32_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
f64_t Stream::readF64(const f64_t& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
bool Stream::readBool(const bool& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
String Stream::readString(const String& defaultValue)
{
    return ReadWithDefaultValue(*this, defaultValue);
}
//============================================================
bool Stream::read(i8_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(u8_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(i16_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(u16_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(i32_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(u32_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(i64_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(u64_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(f32_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(f64_t& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(bool& value)
{
	return ReadValue(*this, value);
}
bool Stream::read(String& value)
{
	return ReadValue(*this, value);
}
//=============================================================
bool Stream::write(i8_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(u8_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(i16_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(u16_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(i32_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(u32_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(i64_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(u64_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(f32_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(f64_t value)
{
	return WriteValue(*this, value);
}
bool Stream::write(bool value)
{
	return WriteValue(*this, value);
}
bool Stream::write(const String& value)
{
	return WriteValue(*this, value);
}
//-----------------------------------------------------------------
_EndNamespace(eokas)
