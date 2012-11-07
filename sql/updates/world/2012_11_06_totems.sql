DELETE FROM spell_bonus_data WHERE entry = 5672;
DELETE FROM spell_bonus_data WHERE entry = 52042;
DELETE FROM spell_bonus_data WHERE entry = 52041;

INSERT INTO spell_bonus_data (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES (52041, 0.0827,-1,-1,-1,"Shaman - Healing Stream Totem"); 