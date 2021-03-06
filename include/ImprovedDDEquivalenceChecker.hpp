/*
 * This file is part of IIC-JKU QCEC library which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum/ for more information.
 */

#ifndef QUANTUMCIRCUITEQUIVALENCECHECKING_IMPROVEDDDEQUIVALENCECHECKER_HPP
#define QUANTUMCIRCUITEQUIVALENCECHECKING_IMPROVEDDDEQUIVALENCECHECKER_HPP

#include <memory>
#include <array>
#include <unordered_set>
#include <chrono>

#include "EquivalenceChecker.hpp"

namespace ec {

	class ImprovedDDEquivalenceChecker: public EquivalenceChecker {

		/// Alternate between LEFT and RIGHT applications
		void checkNaive(qc::permutationMap& perm1, qc::permutationMap& perm2);
		/// Alternate according to the gate count ratio between LEFT and RIGHT applications
		void checkProportional(qc::permutationMap& perm1, qc::permutationMap& perm2);
		/// Look-ahead LEFT and RIGHT and choose the more promising option
		void checkLookahead(qc::permutationMap& perm1, qc::permutationMap& perm2);

	protected:
		decltype(qc1->begin()) it1;
		decltype(qc2->begin()) it2;
		decltype(qc1->end()) end1;
		decltype(qc1->end()) end2;

		std::array<short, qc::MAX_QUBITS> line{};
		Method method = Proportional;

		/// Take operation and apply it either from the left or (inverted) from the right
		/// \param op operation to apply
		/// \param to DD to apply the operation to
		/// \param dir LEFT or RIGHT
		void applyGate(std::unique_ptr<qc::Operation>& op, dd::Edge& e, std::map<unsigned short, unsigned short>& permutation, Direction dir = LEFT);

		unsigned long long nodecount(const dd::Edge& e, std::unordered_set<dd::NodePtr>& visited) {
			visited.insert(e.p);
			unsigned long long sum = 1;
			if(!dd->isTerminal(e)) {
				for (const auto& edge: e.p->e) {
					if (edge.p != nullptr && !visited.count(edge.p)) {
						sum += nodecount(edge, visited);
					}
				}
			}
			return sum;
		}

	public:
		ImprovedDDEquivalenceChecker(qc::QuantumComputation& qc1, qc::QuantumComputation& qc2, Method method = Proportional):
				EquivalenceChecker(qc1, qc2), method(method){
			line.fill(qc::LINE_DEFAULT);
			results.method = method;
		}

		Method getMethod() {
			return  method;
		}

		void setMethod(Method m) {
			method = results.method = m;
		}

		/// Use dedicated method to check the equivalence of both provided circuits
		void check(const Configuration& config) override;
	};

}


#endif //QUANTUMCIRCUITEQUIVALENCECHECKING_IMPROVEDDDEQUIVALENCECHECKER_HPP
