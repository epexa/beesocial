// visual actions

let $mainPage = document.querySelector('#main-page');
let $projectsPage = document.querySelector('#projects-page');
let $projects = $projectsPage.querySelector('#projects');
let $projectModal = document.getElementById('project-modal');
let projectModal = new Modal($projectModal);
let $purchasedResourcesPage = document.querySelector('#purchased-resources-page');
let $purchasedResourcesTbody = document.querySelector('#purchased-resources tbody');
let $resourcesPage = document.querySelector('#resources-page');
let $resources = $resourcesPage.querySelector('#resources');
let $createResourceModal = document.getElementById('create-resource-modal');
let createResourceModal = new Modal($createResourceModal);
let $resourceModal = document.getElementById('resource-modal');
let resourceModal = new Modal($resourceModal);
let $createProjectModal = document.getElementById('create-project-modal');
let createProjectModal = new Modal($createProjectModal);
let $resourceItem = document.querySelector('#resource-item');
let modalAuth = new Modal(document.getElementById('auth'));
let editNpoModal = new Modal(document.getElementById('edit-npo-modal'));
let editSponsorsModal = new Modal(document.getElementById('edit-sponsors-modal'));
let modalSignin = new Modal(document.getElementById('signin-modal'));
let modalReviewEmployee = new Modal(document.getElementById('review-employee'));
let modalDonateNpos = new Modal(document.getElementById('donation-npos-modal'));
let modalDescriptionNpos = new Modal(document.getElementById('description-npos-modal'));
let resourceItemSelected;

let arr = document.querySelector('#projects').querySelectorAll('.btn');
arr.forEach(function (item, i, arr) {
	arr[i].addEventListener('click', function () {
		projectModal.show(); // projects modals
	});
});

arr = document.getElementById('encouraging-employees').querySelectorAll('.review');
arr.forEach(function (item, i, arr) {
	arr[i].addEventListener('click', function () {
		modalReviewEmployee.show();
	});
});

arr = document.getElementById('donation-npos').querySelectorAll('.donate');
arr.forEach(function (item, i, arr) {
	arr[i].addEventListener('click', function () {
		modalDonateNpos.show();
	});
});

arr = document.getElementById('donation-npos').querySelectorAll('.description');
arr.forEach(function (item, i, arr) {
	arr[i].addEventListener('click', function () {
		modalDescriptionNpos.show();
	});
});

window.addEventListener('hashchange', function () {
	let hash = window.location.hash.substring(1);
	if (hash) {
		let params = hash.split('/');
		console.log(params);
		if (params[0]) {
			switch (params[0]) {
				case 'resources':
					{
						$mainPage.style.display = 'none';
						$purchasedResourcesPage.style.display = 'none';
						$projectsPage.style.display = 'none';
						$resourcesPage.style.display = 'block';
						//getResources();
					};
					break;
				case 'projects':
					{
						$mainPage.style.display = 'none';
						$purchasedResourcesPage.style.display = 'none';
						$resourcesPage.style.display = 'none';
						$projectsPage.style.display = 'block';
					};
					break;
				case 'purchased-resources':
					{
						$mainPage.style.display = 'none';
						$resourcesPage.style.display = 'none';
						$projectsPage.style.display = 'none';
						$purchasedResourcesPage.style.display = 'block';
						showPurchasedResources();
					};
					break;
			}
		}
	} else {
		$purchasedResourcesPage.style.display = 'none';
		$mainPage.style.display = 'block';
	}
});

document.querySelector('#avatar').addEventListener("focusin", function () {
	document.querySelector('#dropdown-avatar').classList.add('show');
});

document.querySelector('#dropdown-avatar').addEventListener('click', function () {
	document.querySelector('#dropdown-avatar').classList.remove('show');
});

document.getElementById('create-project-btn').addEventListener('click', function () {
	createProjectModal.show();
});

document.getElementById('create-resource-btn').addEventListener('click', function () {
	createResourceModal.show();
});

document.getElementById('login').addEventListener('click', function () {
	modalAuth.show();
});

document.getElementById('signin').addEventListener('click', function () {
	modalSignin.show();
});

window.dispatchEvent(new CustomEvent('hashchange'));
