
#include "time.h"
#include "string.h"
#include <limits>

_BeginNamespace(eokas)
/*
============================================================================================
==== TimeSpan
============================================================================================
*/
const i64_t TimeSpan::kUsecsPerMsec = 1000;
const i64_t TimeSpan::kUsecsPerSecond = 1000000;
const i64_t TimeSpan::kUsecsPerMinute = 60000000;
const i64_t TimeSpan::kUsecsPerHour = 3600000000;
const i64_t TimeSpan::kUsecsPerDay = 86400000000;

TimeSpan TimeSpan::maxValue()
{
	return TimeSpan(LLONG_MAX);
}

TimeSpan TimeSpan::minValue()
{
	return TimeSpan(LLONG_MIN);
}

TimeSpan::TimeSpan(i64_t usecs)
	:mSpan(usecs)
{}

TimeSpan::TimeSpan(i32_t days, i32_t hours, i32_t mins, i32_t secs, i32_t msecs, i32_t usecs)
	:mSpan(kUsecsPerDay*days + kUsecsPerHour*hours + kUsecsPerMinute*mins + kUsecsPerSecond*secs + kUsecsPerMsec*msecs + usecs)
{}

TimeSpan::TimeSpan(const TimeSpan& other)
	:mSpan(other.mSpan)
{}
	
TimeSpan::~TimeSpan()
{}

TimeSpan& TimeSpan::operator=(const TimeSpan& other)
{
	mSpan = other.mSpan;
	return *this;
}

TimeSpan TimeSpan::operator+(const TimeSpan& other)
{
	return TimeSpan(mSpan + other.mSpan);
}

TimeSpan TimeSpan::operator-(const TimeSpan& other)
{
	return TimeSpan(mSpan - other.mSpan);
}

TimeSpan& TimeSpan::operator+=(const TimeSpan& other)
{
	mSpan += other.mSpan;
	return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan& other)
{
	mSpan -= other.mSpan;
	return *this;
}

bool TimeSpan::operator>(const TimeSpan& other)
{
	return mSpan > other.mSpan;
}

bool TimeSpan::operator<(const TimeSpan& other)
{
	return mSpan < other.mSpan;
}

bool TimeSpan::operator>=(const TimeSpan& other)
{
	return mSpan >= other.mSpan;
}

bool TimeSpan::operator<=(const TimeSpan& other)
{
	return mSpan <= other.mSpan;
}

bool TimeSpan::operator==(const TimeSpan& other)
{
	return mSpan == other.mSpan;
}

bool TimeSpan::operator!=(const TimeSpan& other)
{
	return mSpan != other.mSpan;
}

i64_t TimeSpan::dayPart() const
{
	return mSpan / kUsecsPerDay;
}

i64_t TimeSpan::hourPart() const
{
	return mSpan % kUsecsPerDay / kUsecsPerHour;
}

i64_t TimeSpan::minutePart() const
{
	return mSpan % kUsecsPerDay % kUsecsPerHour / kUsecsPerMinute;
}

i64_t TimeSpan::secondPart() const
{
	return mSpan % kUsecsPerDay % kUsecsPerHour % kUsecsPerMinute / kUsecsPerSecond;
}

i64_t TimeSpan::millisecondPart() const
{
	return mSpan % kUsecsPerDay % kUsecsPerHour % kUsecsPerMinute % kUsecsPerSecond / kUsecsPerMsec;
}

i64_t TimeSpan::microsecondPart() const
{
	return mSpan % kUsecsPerMsec;
}

i64_t TimeSpan::totalDays() const
{
	i64_t result = mSpan / kUsecsPerDay;
	if(mSpan % kUsecsPerDay > 0)
	{
		result += 1;
	}
	return result;
}

i64_t TimeSpan::totalHours() const
{
	i64_t result = mSpan / kUsecsPerHour;
	if(mSpan % kUsecsPerHour > 0)
	{
		result += 1;
	}
	return result;
}

i64_t TimeSpan::totalMinutes() const
{
	i64_t result = mSpan / kUsecsPerMinute;
	if(mSpan % kUsecsPerMinute > 0)
	{
		result += 1;
	}
	return result;
}

i64_t TimeSpan::totalSeconds() const
{
	i64_t result = mSpan / kUsecsPerSecond;
	if(mSpan % kUsecsPerSecond > 0)
	{
		result += 1;
	}
	return result;
}

i64_t TimeSpan::totalMilliseconds() const
{
	i64_t result = mSpan / kUsecsPerMsec;
	if(mSpan % kUsecsPerMsec > 0)
	{
		result += 1;
	}
	return result;
}

f64_t TimeSpan::exactDays() const
{
	f64_t result = mSpan / (kUsecsPerDay + 0.0);
	return result;
}

f64_t TimeSpan::exactHours() const
{
	f64_t result = mSpan / (kUsecsPerHour + 0.0);
	return result;
}

f64_t TimeSpan::exactMinutes() const
{
	f64_t result = mSpan / (kUsecsPerMinute + 0.0);
	return result;
}

f64_t TimeSpan::exactSeconds() const
{
	f64_t result = mSpan / (kUsecsPerSecond + 0.0);
	return result;
}

f64_t TimeSpan::exactMilliseconds() const
{
	f64_t result = mSpan / (kUsecsPerMsec + 0.0);
	return result;
}

/*
============================================================================================
==== TimePoint
============================================================================================
*/
TimePoint::TimePoint()
	:mTimeStamp(time(nullptr))
{
	mTimeStruct = localtime(&mTimeStamp);
}

TimePoint::TimePoint(time_t timeStamp)
	:mTimeStamp(timeStamp)
{
	mTimeStruct = localtime(&mTimeStamp);
}

TimePoint::TimePoint(int year, int month, int date, int hour, int minute, int sec)
{
	tm t;
	t.tm_year = year-1900;
	t.tm_mon = month-1;
	t.tm_mday = date;
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = sec;
	mTimeStamp = mktime(&t);
	mTimeStruct = localtime(&mTimeStamp);
}

TimePoint::TimePoint(const TimePoint& other)
	:mTimeStamp(other.mTimeStamp)
{
	mTimeStruct = localtime(&mTimeStamp);
}

TimePoint::~TimePoint()
{}

TimePoint& TimePoint::operator=(const TimePoint& other)
{
	mTimeStamp = other.mTimeStamp;
	mTimeStruct = localtime(&mTimeStamp);
	return *this;
}

TimePoint TimePoint::operator+(const TimeSpan& span)
{
	return TimePoint(time_t(mTimeStamp + span.totalSeconds()));
}

TimePoint TimePoint::operator-(const TimeSpan& span)
{
	return TimePoint(time_t(mTimeStamp - span.totalSeconds()));
}

TimePoint& TimePoint::operator+=(const TimeSpan& span)
{
	mTimeStamp += span.totalSeconds();
	mTimeStruct = localtime(&mTimeStamp);
	return *this;
}

TimePoint& TimePoint::operator-=(const TimeSpan& span)
{
	mTimeStamp -= span.totalSeconds();
	mTimeStruct = localtime(&mTimeStamp);
	return *this;
}

TimeSpan TimePoint::operator-(const TimePoint& other)
{
	return TimeSpan((mTimeStamp - other.mTimeStamp)*TimeSpan::kUsecsPerSecond);
}

bool TimePoint::operator==(const TimePoint& other)
{
	return mTimeStamp == other.mTimeStamp;
}

bool TimePoint::operator!=(const TimePoint& other)
{
	return mTimeStamp != other.mTimeStamp;
}

bool TimePoint::operator>(const TimePoint& other)
{
	return mTimeStamp > other.mTimeStamp;
}

bool TimePoint::operator<(const TimePoint& other)
{
	return mTimeStamp < other.mTimeStamp;
}

bool TimePoint::operator>=(const TimePoint& other)
{
	return mTimeStamp >= other.mTimeStamp;
}

bool TimePoint::operator<=(const TimePoint& other)
{
	return mTimeStamp <= other.mTimeStamp;
}

time_t TimePoint::calendarTime() const
{
	return mTimeStamp;
}

i32_t TimePoint::year() const
{
	return mTimeStruct->tm_year + 1900;
}

i32_t TimePoint::month() const
{
	return mTimeStruct->tm_mon + 1;
}

i32_t TimePoint::date() const
{
	return mTimeStruct->tm_mday;
}

i32_t TimePoint::hour() const
{
	return mTimeStruct->tm_hour;
}

i32_t TimePoint::minute() const
{
	return mTimeStruct->tm_min;
}

i32_t TimePoint::second() const
{
	return mTimeStruct->tm_sec;
}

i32_t TimePoint::dayOfWeek() const
{
	return mTimeStruct->tm_wday;
}

i32_t TimePoint::dayofYear() const
{
	return mTimeStruct->tm_yday;
}

bool TimePoint::isLeapYear() const
{
	i32_t year = this->year();
	return (year % 4 == 0) || (year % 400 == 0);
}

String TimePoint::toString(const String& fmt)
{
	String result = fmt;

	result = result.replace("YYYY", String::format("%04d", this->year()));
	result = result.replace("YYY",  String::format("%03d", this->year()%1000));
	result = result.replace("YY",   String::format("%02d", this->year()%100));
	result = result.replace("Y",    String::format("%d",   this->year()));
	
	result = result.replace("MM",   String::format("%02d", this->month()));
	result = result.replace("M",    String::format("%d",   this->month()));

	result = result.replace("DD",   String::format("%02d", this->date()));
	result = result.replace("D",    String::format("%d",   this->date()));

	result = result.replace("hh",   String::format("%02d", this->hour()));
	result = result.replace("h",    String::format("%d",   this->hour()));

	result = result.replace("mm",   String::format("%02d", this->minute()));
	result = result.replace("m",    String::format("%d",   this->minute()));

	result = result.replace("ss",   String::format("%02d", this->second()));
	result = result.replace("s",    String::format("%d",   this->second()));
	
	return result;
}

_EndNamespace(eokas)
