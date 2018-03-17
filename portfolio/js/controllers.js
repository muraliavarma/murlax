function ProjectCtrl($scope, $http) {

	angular.module('myPortfolio', []);

	$http.get('data/categories.json').success(function(data) {
		$scope.categories = data;
		//default load
		$scope.setDetails($scope.categories[0].projects[0]);
	});


	$scope.setDetails = function(project) {
		$scope.setSelected(project.id);
		$http.get('data/' + project.id + '.json').success(function(data) {
			$scope.details = data;
			$scope.details.featured = project.featured;
		}).error(function(data, status, headers, config) {
			$scope.details = {};
		});
	};

	$scope.setSelected = function(projectId) {
		for (var i = 0; i < $scope.categories.length; i++) {
			for (var j = 0; j < $scope.categories[i].projects.length; j++) {
				$scope.categories[i].projects[j].selected = ($scope.categories[i].projects[j].id == projectId);
			}
		}
	};

}