#!/bin/bash

source common.sh

cleos wallet open
cleos wallet unlock --password `cat $DEFAULT_PASSWORD_F`

cleos set code beesocial $BEESOCIAL_CONTRACTS_D/beesocial.wast -p beesocial
cleos set abi beesocial $BEESOCIAL_CONTRACTS_D/beesocial.abi -p beesocial
