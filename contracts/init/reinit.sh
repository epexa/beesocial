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
cleos create account beesocial bob `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos create account beesocial dave `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos create account beesocial greg `cat $USER_KEY_F` `cat $USER_KEY_F`

cleos create account beesocial npoa `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos create account beesocial npob `cat $USER_KEY_F` `cat $USER_KEY_F`

cleos create account beesocial sponsora `cat $USER_KEY_F` `cat $USER_KEY_F`
cleos create account beesocial sponsorb `cat $USER_KEY_F` `cat $USER_KEY_F`

cleos push action eosio.token create '[ "beesocial", "1000000000.0000 BEEOS", 0, 0, 0]' -p eosio.token
