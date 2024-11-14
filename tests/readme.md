# Test files

It is a common practice to do unit tests of each class before you integrate it into the project to validate its operation.
In this folder, you can create your own unit test files to validate the operation of your components.

> It might be a good idea to also take some notes about the tests since you are required to 
  report these in the final report.

# Unit Tests

## Test of MyClass

**Involved Classes:**

**Test File:**

**Results:**

## Test of interaction of Action, Item and Game classes

**Involved Classes:** Game, Action, Item, Team, Turn

**Test File:** action_test.cpp

**Results:** The functions and interactions worked as expected. Units were added to a team, weapons given to units.
Multiple turns enqueued with weapon's actions and then executed (prints the weapon's values)

## Test of movement with Map

**Involved Classes:** Map, Terrain

**Test File:** movement_test.cpp

**Results:**
Movement calculating algorithms work properly, taking into account walls and tiles that cost more to move into.
