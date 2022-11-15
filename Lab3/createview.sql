DROP VIEW FullBattlePointsView;

CREATE VIEW FullBattlePointsView AS
    SELECT c.memberID AS memberID, c.role AS role, c.name AS name, SUM(t.extraBattlePoints) + r.battlePoints AS extraBattlePoints
    FROM Things t, Characters c, Roles r
    WHERE (c.memberID, c.role) = (t.ownerMemberID, t.ownerRole)
        AND r.role = c.role
    GROUP BY c.memberID, c.role, r.battlePoints
    HAVING COUNT(t.thingID) >= 1;