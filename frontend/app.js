let options = {
	httpEndpoint: 'http://192.168.43.84:8888',
	debug: true
};
var eos = Eos(options);

eos.getInfo((err, info) => {
	console.log(err, info);
});