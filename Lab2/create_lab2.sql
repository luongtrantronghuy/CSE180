
-- CSE 180 Fall 2022 Lab1 Solution

-- Following two lines are not needed in your solution.
DROP SCHEMA Lab2 CASCADE;
CREATE SCHEMA Lab2;
-- ALTER ROLE htluong SET SEARCH_PATH to Lab2;

-- Members(memberID, name, address, joinDate, expirationDate, isCurrent)  
CREATE TABLE Members (
    memberID INT PRIMARY KEY,
    name VARCHAR(50),
    address VARCHAR(50),
    joinDate DATE,
    expirationDate Date,
    isCurrent BOOLEAN,
    UNIQUE (name, address)
);

-- Rooms(roomID, roomDescription, northNext, eastNext, southNext, westNext)
CREATE TABLE Rooms(
    roomID INT PRIMARY KEY,
    roomDescription VARCHAR(30) NOT NULL,
    northNext INT REFERENCES Rooms(roomID),
    eastNext INT REFERENCES Rooms(roomID),
    southNext INT REFERENCES Rooms(roomID),
    westNext INT REFERENCES Rooms(roomID)
);

-- Roles(role, battlePoints, InitialMoney)
CREATE TABLE Roles (
    role VARCHAR(6) PRIMARY KEY,
    battlePoints INT,
    initialMoney NUMERIC(5,2)
);

-- Characters(memberID, role, name, roomID, currentMoney, wasDefeated)
CREATE TABLE Characters (
    memberID INT,
    role VARCHAR(6) ,
    name VARCHAR(50),
    roomID INT NOT NULL,
    currentMoney NUMERIC(5,2),
    wasDefeated BOOLEAN,
    PRIMARY KEY (memberID, role),
    FOREIGN KEY (memberID) REFERENCES Members,
    FOREIGN KEY (role) REFERENCES Roles,
    FOREIGN KEY (roomID) REFERENCES Rooms,
    UNIQUE (name)
);

-- Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
CREATE TABLE Things (
    thingID INT PRIMARY KEY,
    thingKind CHAR(2),
    initialRoomID INT,
    ownerMemberID INT,
    ownerRole VARCHAR(6),
    cost NUMERIC(4,2) NOT NULL,
    extraBattlePoints INT,
    FOREIGN KEY (ownerMemberID, ownerRole) REFERENCES Characters(memberID, role),
    FOREIGN KEY (initialRoomID) REFERENCES Rooms(roomID)
);

-- Monsters(monsterID, monsterKind, name, battlePoints, roomID, wasDefeated)
CREATE TABLE Monsters (
    monsterID INT PRIMARY KEY,
    monsterKind CHAR(2),
    name VARCHAR(50),
    battlePoints INT,
    roomID INT REFERENCES Rooms,
    wasDefeated BOOLEAN,
    UNIQUE (monsterKind, name)
);

-- Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
CREATE TABLE Battles(
    characterMemberID INT,
    characterRole VARCHAR(6),
    characterBattlePoints INT,
    monsterID INT,
    monsterBattlePoints INT,
    PRIMARY KEY (characterMemberID, characterRole, monsterID),
    FOREIGN KEY (characterMemberID, characterRole) REFERENCES Characters(memberID, role),
    FOREIGN KEY (monsterID) REFERENCES Monsters
);
