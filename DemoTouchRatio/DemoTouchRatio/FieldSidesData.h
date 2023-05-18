/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#pragma once

class FieldSidesData
{
private:
	float own;
	float opponent;
	float neutral;

public:
	FieldSidesData(float own, float opponent, float neutral);

	void AddOwn(float own);
	void AddOpponent(float opponent);
	void AddNeutral(float neutral);

	float GetOwn() const;
	float GetOpponent() const;
	float GetNeutral() const;

	float GetTotal() const;

	inline FieldSidesData& operator+=( FieldSidesData const &other );
	inline FieldSidesData& operator-=( FieldSidesData const &other );
	inline FieldSidesData operator+(FieldSidesData const& other) const;
	inline FieldSidesData operator-(FieldSidesData const& other) const;
	inline FieldSidesData operator*( float const& other ) const;
	inline FieldSidesData operator/(float const& other) const;
	inline FieldSidesData operator*(double const &other ) const;
	inline FieldSidesData operator/(double const& other) const;
};

inline FieldSidesData& FieldSidesData::operator+=( FieldSidesData const &other )
{
	own += other.own;
	opponent += other.opponent;
	neutral += other.neutral;
	return *this;
}

inline FieldSidesData &FieldSidesData::operator-=( FieldSidesData const &other )
{
	own -= other.own;
	opponent -= other.opponent;
	neutral -= other.neutral;
	return *this;
}

inline FieldSidesData FieldSidesData::operator+( FieldSidesData const &other ) const
{
	return FieldSidesData( own + other.own, opponent + other.opponent, neutral + other.neutral );
}

inline FieldSidesData FieldSidesData::operator-( FieldSidesData const &other ) const
{
	return FieldSidesData( own - other.own, opponent - other.opponent, neutral - other.neutral );
}

inline FieldSidesData FieldSidesData::operator*( float const& other ) const
{
	return FieldSidesData( own * other, opponent * other, neutral * other );
}

inline FieldSidesData FieldSidesData::operator/( float const &other ) const
{
	return FieldSidesData( own / other, opponent / other, neutral / other );
}

inline FieldSidesData FieldSidesData::operator*(double const& other) const
{
	return FieldSidesData(own * other, opponent * other, neutral * other);
}

inline FieldSidesData FieldSidesData::operator/(double const& other) const
{
	return FieldSidesData(own / other, opponent / other, neutral / other);
}