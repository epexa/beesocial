let options = {
	httpEndpoint: 'http://192.168.43.84:8888',
	debug: true
};
var eos = Eos(options);

eos.getInfo((err, info) => {
	console.log(err, info);
});

eos.getCode({ account_name: 'beesocial' }, (err, res) => {
	if ( ! err) {
		console.log(res.abi);
	}
	else console.error(err);
});

let resourceItemSelected;
let $resourcesPage = document.querySelector('#resources-page');
let $resources = $resourcesPage.querySelector('#resources');
let $resourceItem = document.querySelector('#resource-item');

eos.getTableRows({
	scope: 'beesocial',
	code: 'beesocial',
	table: 'skill',
	json: true
}, (err, res) => {
	if ( ! err) {
		res.rows.forEach(item => {
			console.log('item', item);
			
			let $newItem = $resourceItem.cloneNode(true);
			$newItem.querySelector('.card-title').innerHTML = item.title;
			$newItem.querySelector('.card-text').innerHTML = item.description;
			$newItem.setAttribute('data-id', item.id);
			$newItem.querySelector('button').addEventListener('click', function() {
				resourceItemSelected = item;
				$resourceModal.querySelector('#resource-title').innerHTML = item.title;
				$resourceModal.querySelector('#resource-description').innerHTML = item.description;
				$resourceModal.querySelector('#resource-how-get').innerHTML = item.howget;
				$resourceModal.querySelector('#resource-contacts').innerHTML = item.contacts;
				$resourceModal.querySelector('#resource-price').innerHTML = item.price;
				resourceModal.show();
			});
			$newItem.style.display = 'block';
			$resources.appendChild($newItem);
			
		});
	}
	else console.error(err);
});