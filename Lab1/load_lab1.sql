--Script to populate the Adventure schema for F11 CSE 180 Lab1


-- Members(memberID, name, address, joinDate, expirationDate, isCurrent)
COPY Members FROM stdin USING DELIMITERS '|';
101|Kendall Lee|123 Main St, New York, NY|2018-01-09|2022-01-09|FALSE
120|Tom Johnson|333 Meder Dr, St. Paul, MN|2011-03-21|2023-04-21|TRUE
103|Siobhan Roy|9931 El Camino, New York, NY|2020-02-12|2022-12-16|TRUE
104|Greg Hirsch|831 Santa Cruz Ave, Toronto, ON|2021-06-30|2024-03-15|TRUE
111|Roman Chan|123 Main St, New York, NY|2018-12-17|2023-12-017|TRUE
109|Gerri Kellman|222 Emmet Grade, Newark, NJ|2016-07-31|2023-06-02|TRUE
131|Stewart Rossellini|11 Private Dr, New York, NY|2022-01-04|2022-12-28|TRUE
100|Logan Schwartz|101 W34 St, New York, NY|2018-05-16|2024-05-16|TRUE
\.

-- Rooms(roomID, roomDescription, northNext, eastNext, southNext, westNext)
COPY Rooms FROM stdin USING DELIMITERS '|';
1|Hall of Mountain King|2|5|4|3
2|Witts End|\N|\N|1|\N
3|Twopit Room|\N|1|\N|5
4|Misty Cavern|1|\N|\N|\N
5|Barren Room|\N|3|\N|1
\.

-- Roles(role, battlePoints, InitialMoney)
COPY Roles FROM stdin USING DELIMITERS '|';
knight|30|62.34
mage|40|95.11
cleric|10|48.97
rogue|20|150.00
\.

-- Characters(memberID, role, name, roomID, currentMoney, wasDefeated)
COPY Characters FROM stdin USING DELIMITERS '|';
101|knight|Lancelot|1|62.34|FALSE
101|rogue|Bilbo|3|100.25|TRUE
101|mage|Merlin|3|82.34|FALSE
111|cleric|Patrick|1|48.97|FALSE
100|knight|Galahad|2|55.11|FALSE
103|mage|Morgan|5|95.11|TRUE
\.

-- Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
COPY Things FROM stdin USING DELIMITERS '|';
6002|sc|1|\N|\N|7.32|5
6049|sw|3|101|knight|20.25|18
6075|sh|5|111|cleric|14.99|12
6021|ma|2|\N|\N|9.00|6
6034|sc|3|111|cleric|8.42|8
6005|st|4|101|rogue|17.76|10
\.

-- Monsters(monsterID, monsterKind, name, battlePoints, roomID, wasDefeated)
COPY Monsters FROM stdin USING DELIMITERS '|';
944|gi|Unfriendly Giant|72|5|FALSE
963|ch|Bob|24|3|TRUE
965|ba|Basil|46|4|FALSE
925|gi|Fee Fie|60|3|FALSE
988|mi|Bullseye|53|1|FALSE
971|ga|Gargamel|28|2|TRUE
\.

-- Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
COPY Battles FROM stdin USING DELIMITERS '|';
101|knight|30|963|24
103|mage|25|925|46
101|rogue|20|944|72
\.
