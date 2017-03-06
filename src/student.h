/*
 * Copyright (c) 2017 The Hive Bot project authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * student.h - Student header.
 *
 * Student data type and utilities.
 */

#ifndef HIVE_BOT_STUDENT_H_
#define HIVE_BOT_STUDENT_H_

#include <string>

#include "connection.h"
#include "ship.h"

namespace hive_bot {

/**
 * Represents a fellow UWE student.
 */
class Student {

  private:
    // The order to load tick packets sent from this student.
    int load_order_;

    // How far X to offset from the group center when moving.
    int offset_x_;

    // How far Y to offset from the group center when moving.
    int offset_y_;

    // The current predicted score.
    int score_ = 0;

    // The UWE student identity code.
    std::string id_;

    // The students forename.
    std::string forename_;

    // The students surname.
    std::string surname_;

    // The students IP address.
    std::string ip_;

    // The students ship details.
    Ship ship_;

    // The students connection details.
    Connection connection_;

    // Has the student sent us any information this tick?
    bool connected_ = false;

  public:
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
    Student(int load_order, int offset_x, int offset_y, std::string id,
      std::string forename, std::string surname, std::string ip);

    /**
     * Gets the order to load tick packets sent from this student.
     *
     * @return the load order.
     */
    int GetLoadOrder();

    /**
     * Gets how far X this student is offset from the group center when moving.
     *
     * @return the X movement offset.
     */
    int GetOffsetX();

    /**
     * Gets how far Y this student is offset from the group center when moving.
     *
     * @return the Y movement offset.
     */
    int GetOffsetY();

    /**
     * Gets the current predicted score for this student.
     *
     * @return the score.
     */
    int GetScore();

    /**
     * Sets the current predicted score for this student.
     *
     * @param score The predicted score.
     */
    void SetScore(int score);

    /**
     * Gets the UWE student identity code.
     *
     * @return the ID.
     */
    std::string GetId();

    /**
     * Gets the students forename.
     *
     * @return the forename.
     */
    std::string GetForename();

    /**
     * Gets the students surname.
     *
     * @return the surname.
     */
    std::string GetSurname();

    /**
     * Gets the students IP address.
     *
     * @return the IP.
     */
    std::string GetIp();

    /**
     * Gets the students ship details.
     *
     * @return the ship.
     */
    Ship GetShip();

    /**
     * Sets the students ship details.
     *
     * @param ship The updated students ship details.
     */
    void SetShip(Ship ship);

    /**
     * Gets the students connection.
     *
     * @return the connection;
     */
    Connection GetConnection();

    /**
     * Sets the students connection.
     *
     * @param connection The updated students connection.
     */
    void SetConnection(Connection connection);

    /**
     * Determines whether or not we have received any information from this
     * student in the current tick.
     *
     * @return {@code true} if they're connected, otherwise {@code false}.
     */
    bool IsConnected();

    /**
     * Sets if the student is connected or not.
     *
     * @param connected  {@code true} if they're connected, otherwise
     *                   {@code false}.
     */
    void SetConnected(bool connected);
};

} // namespace hive_bot

#endif // HIVE_BOT_STUDENT_H_
