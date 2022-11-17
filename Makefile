#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
#
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#

target: build

.PHONY: init
init:
	mkdir -p build
	cd build && cmake ..

build: init
	cd build && make -j4

.PHONY: build_test
build_test:
	mkdir -p build_test
	cd build_test && cmake .. -DPICOLATOR_TEST=1
	cd build_test && make -j4

run_test: build_test
	./build_test/test/picolator_test
