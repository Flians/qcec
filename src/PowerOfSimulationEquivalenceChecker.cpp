/*
 * This file is part of IIC-JKU QCEC library which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum/ for more information.
 */

#include "PowerOfSimulationEquivalenceChecker.hpp"

namespace ec {

	void PowerOfSimulationEquivalenceChecker::check(const Configuration& config) {
		results.method = PowerOfSimulation;
		fidelity_limit = config.fidelity_limit;
		max_sims = config.max_sims;
		results.nsims = 0;

		#if DEBUG_MODE_SIMULATION
		std::cout << "Simulating max. " << max_sims << " times with target fidelity " << fidelity_limit << std::endl;
		#endif

		if (!validInstance())
			return;

		auto start = std::chrono::high_resolution_clock::now();
		while (results.nsims < max_sims) {
			// generate distinct stimulus
			auto new_stimulus = stimuli.insert(stimuliGenerator());
			while (!new_stimulus.second) {
				new_stimulus = stimuli.insert(stimuliGenerator());
			}
			results.basisState = *new_stimulus.first;
			std::bitset<64> stimulusBits(results.basisState);

			#if DEBUG_MODE_SIMULATION
			std::cout << "\033[32mSim " << results.nsims << ": " << results.basisState << "\033[0m" << std::endl;
			#endif

			dd::Edge in1 = dd->makeBasisState(qc1->getNqubits(), stimulusBits);
			auto e = qc1->simulate(in1, dd);

			#if DEBUG_MODE_SIMULATION
			std::stringstream ss1 {};
			ss1 << "e_" << filename1 << "_sim_" << results.nsims << ".dot";
			dd->export2Dot(e, ss1.str().c_str(), true);
			std::cout << "[after 1st circ] Complex count: " << dd->cn.count << std::endl;
			#endif

			dd::Edge in2 = dd->makeBasisState(qc2->getNqubits(), stimulusBits);
			auto f = qc2->simulate(in2, dd);

			#if DEBUG_MODE_SIMULATION
			std::stringstream ss2 {};
			ss2 << "f_" << filename1 << "_sim_" << results.nsims << ".dot";
			dd->export2Dot(f, ss2.str().c_str(), true);
			std::cout << "[after 2nd circ] Complex count: " << dd->cn.count << std::endl;
			#endif

			results.fidelity = dd->fidelity(e, f);

			#if DEBUG_MODE_SIMULATION
			std::cout << "\033[33mFidelity: " << results.fidelity << "\033[0m" << std::endl;
			#endif

			results.nsims++;
			results.maxActive = std::max(dd->maxActive, results.maxActive);

			if (results.fidelity < fidelity_limit) {
				results.equivalence = NonEquivalent;
				dd->decRef(e);
				dd->decRef(f);
				dd->garbageCollect(true);

				#if DEBUG_MODE_SIMULATION
				std::cout << "[after success] Complex count: " << dd->cn.count << std::endl;
				if (dd->cn.count > 4) {
					std::cout << "\033[33m!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\033[0m" << std::endl;
				}
				#endif
				break;
			} else if (results.nsims == (unsigned long long) std::pow((long double)2, nqubits_for_stimuli)) {
				results.equivalence = Equivalent;
				dd->decRef(e);
				dd->decRef(f);
				dd->garbageCollect(true);

				#if DEBUG_MODE_SIMULATION
				std::cout << "[after success] Complex count: " << dd->cn.count << std::endl;
				#endif
				break;
			} else {
				results.equivalence = ProbablyEquivalent;
				dd->decRef(e);
				dd->decRef(f);
				dd->garbageCollect(true);

				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff = end - start;
				results.time = diff.count();
				if (config.printCSV) {
					printCSVEntry(std::cout);
					std::cout << std::flush;
				}
				#if DEBUG_MODE_SIMULATION
				std::cout << "[end of sim] Complex count: " << dd->cn.count << std::endl;
				#endif
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		results.time = diff.count();
	}

}
