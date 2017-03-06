/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * student.cc - Student implementation.
 *
 * Student data type and utilities.
 */

#include "student.h"

namespace hive_bot {

/**
 * Constructs a new student.
 *
 * @param load_order The order to load tick packets sent from this student.
 * @param offset_x   How far X to offset from the group center when moving.
 * @param offset_Y   How far Y to offset from the group center when moving.
 * @param id         The UWE student identity code.
 * @param forename   The students forename.
 * @param surname    The students surname.
 * @param ip         The students IP address.
 */
Student::Student(int load_order, int offset_x, int offset_y, std::string id,
    std::string forename, std::string surname, std::string ip) {
  load_order_ = load_order;
  offset_x_ = offset_x;
  offset_y_ = offset_y;
  id_ = id;
  forename_ = forename;
  surname_ = surname;
  ip_ = ip;
}

/**
 * Gets the order to load tick packets sent from this student.
 *
 * @return the load order.
 */
int Student::GetLoadOrder() {
  return load_order_;
}

/**
 * Gets how far X this student is offset from the group center when moving.
 *
 * @return the X movement offset.
 */
int Student::GetOffsetX() {
  return offset_x_;
}

/**
 * Gets how far Y this student is offset from the group center when moving.
 *
 * @return the Y movement offset.
 */
int Student::GetOffsetY() {
  return offset_y_;
}

/**
 * Gets the current predicted score for this student.
 *
 * @return the score.
 */
int Student::GetScore() {
  return score_;
}

/**
 * Sets the current predicted score for this student.
 *
 * @param score The predicted score.
 */
void Student::SetScore(int score) {
  score_ = score;
}

/**
 * Gets the UWE student identity code.
 *
 * @return the ID.
 */
std::string Student::GetId() {
  return id_;
}

/**
 * Gets the students forename.
 *
 * @return the forename.
 */
std::string Student::GetForename() {
  return forename_;
}

/**
 * Gets the students surname.
 *
 * @return the surname.
 */
std::string Student::GetSurname() {
  return surname_;
}

/**
 * Gets the students IP address.
 *
 * @return the IP.
 */
std::string Student::GetIp() {
  return ip_;
}

/**
 * Gets the students ship details.
 *
 * @return the ship.
 */
Ship Student::GetShip() {
  return ship_;
}

/**
 * Sets the students ship details.
 *
 * @param ship The updated students ship details.
 */
void Student::SetShip(Ship ship) {
  ship_ = ship;
}

/**
 * Gets the students connection.
 *
 * @return the connection;
 */
Connection Student::GetConnection() {
  return connection_;
}

/**
 * Sets the students connection.
 *
 * @param connection The updated students connection.
 */
void Student::SetConnection(Connection connection) {
  connection_ = connection;
}

/**
 * Determines whether or not we have received any information from this
 * student in the current tick.
 *
 * @return {@code true} if they're connected, otherwise {@code false}.
 */
bool Student::IsConnected() {
  return connected_;
}

/**
 * Sets if the student is connected or not.
 *
 * @param connected  {@code true} if they're connected, otherwise
 *                   {@code false}.
 */
void Student::SetConnected(bool connected) {
  connected_ = connected;
}

} // namespace hive_bot
