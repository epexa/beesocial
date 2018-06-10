const Eos = require('eosjs');

let options = {
	httpEndpoint: 'http://192.168.43.240:8888',
	debug: false,
	keyProvider: '5JBx24Y4w7BvA35JnScvhMYCvRjWoheKKQ6dt1R12ZV4PqB55Gh',
	//authorization: 'alice@active',
	sign: true,
	//broadcast: true
};
var eos = Eos(options);


/* eos.getCode({ account_name: 'beesocial' }, (err, res) => {
	console.log('actions', res.abi.actions);
	console.log('tables', res.abi.tables);
	console.log('structs', res.abi.structs);
}); */


eos.transaction('beesocial', (operation) => {
	operation.worker('erlan', 'Yerlan Shindauletau', 'BY, Minsk', '2016-09-05T20:15:11', [1, 2, 3], 1, {authorization: 'beesocial'}); // alice@active
});

