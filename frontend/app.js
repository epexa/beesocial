let options = {
	httpEndpoint: 'http://192.168.43.84:8888',
	debug: true,
	keyProvider: '5Jmsawgsp1tQ3GD6JyGCwy1dcvqKZgX6ugMVMdjirx85iv5VyPR', // PW5KATTYpBNPdj4BAqGoBGBntQGtdyzdY3xn45TCZqso1cBtGn3k5
	//authorization: 'alice@active',
	sign: true,
	//broadcast: true
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

let $loader = document.getElementsByClassName('lding')[0];
let loadingShow = function() {
	$loader.style.display = 'block';
};
let loadingHide = function() {
	$loader.style.display = 'none';
};

/* let resourceItemSelected;
let $resourcesPage = document.querySelector('#resources-page');
let $resources = $resourcesPage.querySelector('#resources');
let $resourceItem = document.querySelector('#resource-item'); */

eos.getTableRows({
	scope: 'beesocial',
	code: 'beesocial',
	table: 'skill',
	json: true
}, (err, res) => {
	if ( ! err) {
		res.rows.forEach(item => {
			console.log('item', item);
			
			/* let $newItem = $resourceItem.cloneNode(true);
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
			$resources.appendChild($newItem); */
			
		});
	}
	else console.error(err);
});

let modalAuth = new Modal(document.getElementById('auth'));
let $login = document.getElementById('login');
let $logout = document.getElementById('logout');

document.getElementById('form-login-pass').addEventListener('submit', e => {
	e.preventDefault();
	let log = document.getElementById('logged').checked,
		user = document.getElementById('input-user').value,
		pass = document.getElementById('input-pass').value;
	let username = user;
	let wif = pass;
	log ? localStorage.username = username : '';
	log ? localStorage.wif = JSON.stringify(wif) : {};
	$login.style.display = 'none';
	$logout.style.display = 'inline-block';
	modalAuth.hide();
	if (callbackAuth) callbackAuth();
});

let auth = function(callback) {
	if (wif && username) callback();
	else {
		modalAuth.show();
		callbackAuth = callback;
	}
};

//
$login.addEventListener('click', function() {
	modalAuth.show();
});

let $createResourceModalForm = $createResourceModal.querySelector('form');
$createResourceModalForm.addEventListener('submit', function(e) {
	e.preventDefault();
	let title = this.title.value,
		combs = this.combs.value,
		howGet = this.howget.value,
		description = this.description.value,
		contacts = this.contacts.value,
		author = username;
	auth(function() {
		loadingShow();
		eos.transaction('beesocial', (operation) => {
			operation.create(title, 'event', combs + ' HONEYCOMBS', 1, howGet, description, contacts, {authorization: author}); // alice@active
			loadingHide();
			$createResourceModalForm.reset();
			createResourceModal.hide();
			//window.location.hash = '#resources/' + id;
		});
	});
});