INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (879, 'spell_pal_exorcism');
UPDATE spell_bonus_data SET dot_bonus = 0, ap_dot_bonus = 0 WHERE entry = 879;