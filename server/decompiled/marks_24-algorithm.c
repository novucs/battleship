
// When not moving, start moving a random direction.
if (speedX == 0 & speedX == 0) {
  speedX = (int)Math.Round((double)Conversion.Int(unchecked(10f * VBMath.Rnd() + 1f))) - 5;
  speedY = (int)Math.Round((double)Conversion.Int(unchecked(10f * VBMath.Rnd() + 1f))) - 5;
}

// Ensure the ship does not collide with the map edges.
if (shipX > 990) {
  speedX = -2;
} else if (shipX < 10) {
  speedX = 2;
}

if (shipY > 990) {
  speedY = -2;
} if (shipY < 10) {
  speedY = 2;
}

// Declare temporary variables...
int viewedShips = 0; // Number of ships viewed this tick
int targetShip = -1; // Enemy ship ID to target
int lowestDistance = 1000; // Lowest distance to enemy currently found
int tempShipId = 0; // Temporary ship index used when iterating through all ships on the map

// Locate a ship to target.
do {
  if (inuse[tempShipId] && active[tempShipId]) {
    if (tempShipId != 0 && // Do not target our own ship
        ((shipX < 500 && tempShipId != 4 && tempShipId != 5 && tempShipId != 6) || shipX >= 500) && // Do not target ships 4,5,6 (the pack) when our X is less than 500
        Operators.ConditionalCompareObjectLess(this.Distance[0, num4], this.VisibleRange(0, num4), false) && // Ship is within view distance
        lowestDistance > Distance[0, tempShipId]) { // Current ship ID is closest so far
      lowestDistance = Distance[0, tempShipId];
      targetShip = tempShipId;
    }

    viewedShips++;
  }

  tempShipId++;
} while (tempShipId <= 200);

// If target ship is valid.
if (targetShip >= 0) {
  // Fire at the target ship.
  if (fireCount < 10) {
    fire = true;
    fireX = shipX[targetShip];
    fireY = shipY[targetShip];
    fireCount = fireCount + 1;
  } else {
    fireCount = 1;
  }

  // Move towards the target ship.
  if (shipX < shipX[targetShip]) {
    speedX = 2;
  }

  if (shipX > shipX[targetShip]) {
    speedX = -2;
  }

  if (shipY < shipY[targetShip]) {
    speedY = 2;
  }

  if (shipY > shipY[targetShip]) {
    speedY = -2;
  }

  // Use the target ships flag.
  flag = flag[targetShip];
}

moves = true;

// If more than 15 ships are surrounding us.
if (viewedShips > 15) {
  // Get the count of each surrounding ship type.
  int battleshipCount = 0;
  int frigateCount = 0;
  int submarineCount = 0;
  tempShipId = 7;

  do {
    if (ship_type[tempShipId] == 0) {
      battleshipCount++;
    }

    if (ship_type[tempShipId] == 1) {
      frigateCount++;
    }

    if (ship_type[tempShipId] == 2) {
      submarineCount++;
    }

    tempShipId++;
  } while (tempShipId <= 200);

  // Update ship type to counter majority of ships.
  if (battleshipCount > frigateCount + submarineCount) {
    ship_type = 2;
  }

  if (frigateCount > battleshipCount + submarineCount) {
    ship_type = 0;
  }

  if (submarineCount > frigateCount + battleshipCount) {
    ship_type = 1;
  }
}
