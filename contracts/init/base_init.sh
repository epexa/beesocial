#!/bin/bash

source common.sh

mkdir -p "${PASSWORDS_D}"

cleos set contract eosio "${EOS_CONTRACTS_D}/eosio.bios" -p eosio

cleos wallet create | awk '/^"P/ {print substr($0, 2, length($0)-2)}' > $DEFAULT_PASSWORD_F
cleos wallet create_key | awk '{print substr($NF, 2, length($NF)-2)}' > $EOS_TOKEN_KEY_F
cleos wallet create_key | awk '{print substr($NF, 2, length($NF)-2)}' > $BEESOCIAL_KEY_F

cleos wallet open
cleos wallet unlock --password `cat $DEFAULT_PASSWORD_F`

cleos create account eosio eosio.token `cat $EOS_TOKEN_KEY_F` `cat $EOS_TOKEN_KEY_F`
cleos set contract eosio.token "${EOS_CONTRACTS_D}/eosio.token"

cleos create account eosio beesocial `cat $BEESOCIAL_KEY_F` `cat $BEESOCIAL_KEY_F`
