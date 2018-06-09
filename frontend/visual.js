// visual actions

let $mainPage = document.querySelector('#main-page');
let $projectsPage = document.querySelector('#projects-page');
let $projects = $projectsPage.querySelector('#projects');
let $projectModal = document.getElementById('project-modal');
let projectModal = new Modal($projectModal);

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
