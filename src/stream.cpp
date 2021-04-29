
#include "stream.h"
#include "string.h"

_BeginNamespace(eokas)

DataStream::DataStream(Stream& target)
    :target(target)
{}

bool DataStream::open()
{
    return target.open();
}
void DataStream::close()
{
    return target.close();
}
bool DataStream::isOpen() const
{
    return target.isOpen();
}
bool DataStream::readable() const
{
    return target.readable();
}
bool DataStream::writable() const
{
    return target.writable();
}
bool DataStream::eos() const
{
    return target.eos();
}
size_t DataStream::pos() const
{
    return target.pos();
}
size_t DataStream::size() const
{
    return target.size();
}
size_t DataStream::read(void* data, size_t size)
{
    return target.read(data, size);
}
size_t DataStream::write(void* data, size_t size)
{
    return target.write(data, size);
}
bool DataStream::seek(int offset, int origin) // 0:beg, 1:cur, 2:end
{
    return target.seek(offset, origin);
}
void DataStream::flush()
{
    return target.flush();
}

_EndNamespace(eokas)
