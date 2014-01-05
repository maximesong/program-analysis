(function() {
  // endsWith is not available for chrome
  if (typeof String.prototype.endsWith !== 'function') {
    String.prototype.endsWith = function(suffix) {
      return this.indexOf(suffix, this.length - suffix.length) !== -1;
    };
  }
  angular.module("app", [])
    .controller("sourceCtrl", function($scope, $http, $location) {
      var POST_URL = "/serverdata";
      $scope.DEBUG = false;
      $scope.dir = {
	'files':[],
	'dirs':[]
      };
      if (!$location.path()) {
        $location.path("/");
      }

      $scope.debug = function() {
        $scope.DEBUG = !$scope.DEBUG;
      };

      $scope.isCurrentPathADir = function() {
        return $scope.currentPath.endsWith("/");
      };

      $scope.tabSelected = 'Source';

      $scope.onTabSelected = function(tab) {
        $scope.tabSelected = tab;
      };

      $scope.isTabSelected = function(tab) {
        return $scope.tabSelected === tab;
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
	  if ($scope.isCurrentPathADir()) {
            currentDir = newPath;
	    $scope.sourceCode = "";
	  } else {
            currentDir = newPath.replace(/\/[^\/]*$/, '/');
          }
          $http.post(POST_URL,
		     {
                       type: "dir",
                       dir: currentDir
                     }
                    ).success(function (data) {
		      $scope.dir_info = data;
		    });
          if (!$scope.isCurrentPathADir()) {
            $http.post(POST_URL,
		       { 
                         type: "file",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.sourceCode = data;
		      });
            $http.post(POST_URL,
		       { 
                         type: "cfg",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.cfgJSON = data;
		      });
            $http.post(POST_URL,
		       { 
                         type: "cg",
                         "filename": newPath
                       }
                      ).success(function (data) {
		        $scope.cgJSON = data;
//                        data = JSON.stringify(data).replace(/>/g, "").replace(/</g, "");
                        console.log(data);
		        var graph = new Springy.Graph();
                        graph.loadJSON(data);
                        var springy = jQuery('#springydemo').springy({
                          graph: graph
                        });
                      });
	  }
        });
    });
})();
