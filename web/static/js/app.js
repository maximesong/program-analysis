(function() {
  angular.module("app", [])
    .controller("sourceCtrl", function($scope, $http, $location) {
      $scope.dir = {
	'files':[],
	'dirs':[]
      };
      if (!$location.path()) {
        $location.path("/");
      }

      $scope.refresh = function() {
      };

      $scope.is_root_dir = function() {
	return $scope.dir_info && $scope.dir_info.base === $scope.dir_info.parent;
      };

      $scope.$watch(
        function () {
	  return $location.path();
        },
        function (newPath) {
          var currentDir;
          $scope.currentPath = newPath;
	  if (newPath.endsWith("/")) {
            currentDir = newPath;
	  } else {
            currentDir = newPath.replace(/\/[^\/]*$/, '/');
          }
          $http.post("/listdir",
		     { "dir": currentDir }
                    ).success(function (data) {
		      $scope.dir_info = data;
		    });
          if (!newPath.endsWith("/")) {
            $http.post("/serverdata",
		       { 
                         type: "file",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.sourceCode = data;
		      });
	  }
        });
    });
})();
