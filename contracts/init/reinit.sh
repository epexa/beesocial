#!/bin/bash

source common.sh

cleos wallet create | awk '/^"P/ {print substr($0, 2, length($0)-2)}' > $DEFAULT_PASSWORD_F

cleos set contract eosio "${EOS_CONTRACTS_D}/eosio.bios" -p eosio

cleos wallet import `cat $EOS_TOKEN_PRIVATE_KEY_F`
cleos wallet import `cat $BEESOCIAL_PRIVATE_KEY_F`
cleos wallet import `cat $USER_PRIVATE_KEY_F`

cleos create account eosio eosio.token `cat $EOS_TOKEN_KEY_F` `cat $EOS_TOKEN_KEY_F`
cleos set contract eosio.token "${EOS_CONTRACTS_D}/eosio.token"

cleos create account eosio beesocial `cat $BEESOCIAL_KEY_F` `cat $BEESOCIAL_KEY_F`

cleos create account beesocial alice `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission alice active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p alice

cleos create account beesocial bob `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission bob active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p bob

cleos create account beesocial dave `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission dave active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p dave

cleos create account beesocial greg `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission greg active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p greg

cleos create account beesocial npoa `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission npoa active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p npoa

cleos create account beesocial npob `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission npob active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p npob

cleos create account beesocial sponsora `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission sponsora active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p sponsora

cleos create account beesocial sponsorb `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos set account permission sponsorb active '{"threshold": 1,"keys": [{"key": "'`cat /passwords/user.public.key`'","weight": 1}],"accounts": [{"permission":{"actor":"beesocial","permission":"active"},"weight":1}]}' owner -p sponsorb

cleos push action eosio.token create '[ "beesocial", "1000000000.0000 SOCIAL", 0, 0, 0]' -p eosio.token
cleos push action eosio.token issue '[ "beesocial", "100000000.0000 SOCIAL", "" ]' -p beesocial
