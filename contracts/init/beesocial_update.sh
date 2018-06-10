#!/bin/bash

source common.sh

cleos wallet open
cleos wallet unlock --password `cat $DEFAULT_PASSWORD_F`

cleos set code beesocial $BEESOCIAL_CONTRACTS_D/beesocial.wast -p beesocial
cleos set abi beesocial $BEESOCIAL_CONTRACTS_D/beesocial.abi -p beesocial

cleos push action beesocial skill '[ "skill1", true]' -p beesocial
cleos push action beesocial skill '[ "skill2", true]' -p beesocial
cleos push action beesocial skill '[ "skill3", true]' -p beesocial

cleos push action beesocial activity '[ "actiivty1", true]' -p beesocial
cleos push action beesocial activity '[ "activity2", true]' -p beesocial
cleos push action beesocial activity '[ "activity3", true]' -p beesocial

cleos push action beesocial category '[ "category1", 0, true]' -p beesocial
cleos push action beesocial category '[ "category2", 0, true]' -p beesocial
cleos push action beesocial category '[ "category3", 1, true]' -p beesocial

cleos push action beesocial worker  '["alice", "Alice Name", "Alice Location", "1", "1980-01-01T00:00:00", [0, 1, 2], true ]' -p alice
cleos push action beesocial worker  '["bob", "Bob Name", "Bob Location", "0", "1990-01-01T00:00:00", [0, 1, 2], true ]' -p bob
cleos push action beesocial worker  '["dave", "Dave Name", "Dave Location", "0", "1995-01-01T00:00:00", [0, 1, 2], true ]' -p dave
cleos push action beesocial worker  '["greg", "Greg Name", "Greg Location", "0", "1985-01-01T00:00:00", [0, 1, 2], true ]' -p greg

cleos push action beesocial npo '["npoa", "NPO Title 1", "NPO Description 1", "NPO Location 1", true]' -p npoa
cleos push action beesocial npo '["npob", "NPO Title 2", "NPO Description 2", "NPO Location 2", true]' -p npob

cleos push action beesocial sponsor '["sponsora", "SPONSOR Title 1", "SPONSOR Description 1", "SPONSOR Location 1", [0,1], true]' -p sponsora
cleos push action beesocial sponsor '["sponsorb", "SPONSOR Title 2", "SPONSOR Description 2", "SPONSOR Location 2", [2], true]' -p sponsorb

cleos push action beesocial project '["npoa", "NPO 1 Project 1", "NPO DESCRIPTION 1", [0, 1], "2018-07-01T00:00:00", "2018-07-02T00:00:00", "1.0000 SOCIAL", 2]' -p npoa
cleos push action beesocial project '["npob", "NPO 2 Project 2", "NPO DESCRIPTION 2", [1, 2], "2018-07-02T00:00:00", "2018-07-03T00:00:00", "1.0000 SOCIAL", 3]' -p npob

cleos push action beesocial deposit '[0]' -p npoa
cleos push action beesocial cancel '[1]' -p npob


cleos push action beesocial request '["alice",0,0]' -p alice
cleos push action beesocial request '["bob",0,0]' -p bob
cleos push action beesocial request '["dave",0,0]' -p dave
