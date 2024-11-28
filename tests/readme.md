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

## Test of buildings

**Involved Classes:** Game, Map, Building, BuildingPart, BuildingAction, Team, Turn

**Test File:** building_test.cpp

**Results:** Buildings work as expected. You can build them using the building part items, add parts to existing buildings iff it's the right part for
the building and it's not yet added to it. Adding wrong parts does nothing (currently prints message). Using a building returns the right action which gets
executed correctly.

## Test of undoing actions

**Involved Classes:** Game, Action, Team, Map, Building, Unit, Item

**Test File:** action_test.cpp

**Results:** Undoing actions undoes movement, removes part from building (and the building itself if it's left with no parts), and undoes healing (damage
cannot be undone since it's only executed when the whole turn ends). Works as expected

## Test of AoE

**Involved Classes:** Map, Game, Unit, Team, Turn, Weapon, Action

**Test File:** action_test.cpp

**Results:** AoE damage is properly applied to units in AoE range and not applied to those not in range

## Test of Shop

**Involved Classes:** Unit, Team, Weapon, HealingItem

**Test File:** shop_test.cpp

**Results:** The Shop class works as expected. Items can be bought, assigned to Units, retrieved from Units, and refunded back to the Shop. The Shop class forms a team correctly when the form_team() method is called.