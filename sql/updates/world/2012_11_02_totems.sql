-- Usuniecie niepotrzebnych spelli z totemow, zostana one pozniej rzucane przy triggeroweaniu odpowiednich talentow
-- All fire totems: 2523, 5929, 5950, 15439
UPDATE creature_template SET spell2 = 0 WHERE spell2 = 77747;

-- Earthbind totem
UPDATE creature_template SET spell2 = 0 WHERE spell2 = 64695;