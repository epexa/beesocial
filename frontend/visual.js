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
let resourceItemSelected;

let arr = document.querySelector('#projects').querySelectorAll('.btn');
arr.forEach(function(item, i, arr) {
  arr[i].addEventListener('click', function() {
	projectModal.show(); // projects modals
	});
});

window.addEventListener('hashchange', function() {
	let hash = window.location.hash.substring(1);
	if (hash) {
		let params = hash.split('/');
			console.log(params);
		if (params[0]) {
			switch (params[0]) {
				case 'resources': {

				}; break;
				case 'projects': {
					$mainPage.style.display = 'none';
					$projectsPage.style.display = 'block';
				}; break;
				case 'purchased-resources': {

				}; break;
			}
		}
	}
	else {
		$purchasedResourcesPage.style.display = 'none';
		$mainPage.style.display = 'block';
	}
});
