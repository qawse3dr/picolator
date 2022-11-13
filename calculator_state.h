/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once

#include <vector>

class Letter;
struct CalculatorState {
  // Holds a pointer to the current equation (should be index 0)
  // might be able to remove
  std::vector<Letter> cur_equation;
  int cursor;
  float ans;

  // Holds all the equations in memory
  std::vector<std::vector<Letter>> equations;
  int equation_idx;
};
