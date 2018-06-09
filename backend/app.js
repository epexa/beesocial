const Eos = require('eosjs');

let options = {
	httpEndpoint: 'http://192.168.43.240:8888',
	debug: false,
	keyProvider: '5Jmsawgsp1tQ3GD6JyGCwy1dcvqKZgX6ugMVMdjirx85iv5VyPR', // PW5KATTYpBNPdj4BAqGoBGBntQGtdyzdY3xn45TCZqso1cBtGn3k5
	//authorization: 'alice@active',
	sign: true,
	//broadcast: true
};
var eos = Eos(options);


/* eos.transaction('beesocial', (operation) => {
	operation.create(title, 'event', combs + ' HONEYCOMBS', 1, howGet, description, contacts, {authorization: author}); // alice@active
}); */

eos.getCode({ account_name: 'beesocial' }, (err, res) => {
	console.log('actions', res.abi.actions);
	console.log('tables', res.abi.tables);
	console.log('structs', res.abi.structs);
});