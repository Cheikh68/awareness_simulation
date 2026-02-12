#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H
#include <cmath>
#include <algorithm>
#include <random>

enum nodeState { ignorant, aware, active };
enum nodeThreshold { proximity, totalActive, amountOfExposure, influenceBased, passionBased };

struct Node {
	int Y;
	int X;
	nodeState state;
	nodeThreshold thresholdPattern;
	int activityIntent;
	int passion;
	int influence;
	float exposurePressure;
	float exposureDecayRate;
	float awarenessLevel;
	float awarenessDecayRate;
	int range;
	int area;
	float passionFactor;
	int spreadAmount;
	std::vector<Node*> pendingInteractions;
	float energy = 70.0f;
	float energyCost;
	static constexpr float BASE_AWARENESS = 5.0f;
	
	Node()
        : X(0), Y(0), state(ignorant), thresholdPattern(proximity),
          activityIntent(0), passion(0), influence(0)
    {
        recomputeDerived();
    }

    Node(int y, int x, nodeThreshold threshPattern, int activityInt,
         int influenceValue = -1, int passionValue = -1,
         nodeState nodeState = ignorant)
        : Y(y), X(x), state(nodeState),
          thresholdPattern(threshPattern),
          activityIntent(activityInt)
    {
        influence = (influenceValue >= 0)
            ? influenceValue
            : generateInfluence(100, 2.0f);

        passion = (passionValue >= 0)
            ? passionValue
            : generatePassion();

        recomputeDerived();
    }

	void recomputeDerived() {
		range = int(std::sqrt(influence));
		area = (2 * range + 1) * (2 * range + 1) - 1;
		passionFactor = std::pow(passion / 10.0f, 1.5f);
		spreadAmount = int(area * passionFactor);
		exposureDecayRate = awarenessDecayRate = 1.0f * (1.0f - passionFactor);
		energyCost = 1 * (1.2f - passionFactor);
	}

	int generateInfluence(int maxInfluence, float alpha) {
		// u in (0,1], never 0
		float u = (rand() + 1.0f) / (RAND_MAX + 1.0f);

		// Power-law transform
		float value = pow(u, -1.0f / alpha);

		// Scale & clamp
		int influence = int(value);
		return std::min(influence, maxInfluence);
	}

	int generatePassion() {
		float u = random01();
		return int(10 * sqrt(u));
	}

	float random01() {
		static std::mt19937 rng(std::random_device{}());
		static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		return dist(rng);
	}

	void interact(Node* const other, int* totalActiveNodes, int* totalAwareNodes) {
		// First case: this node is ignorant
		if (state == ignorant) {
			state = aware;
			awarenessLevel = BASE_AWARENESS;
			*totalAwareNodes++;
		}

		// Second case: this node is aware
		if (state == aware) {
			switch (thresholdPattern) {
			case proximity:
				if (std::max(abs(X - other->X), abs(Y - other->Y)) <= activityIntent) {
					if (state != active) {								// activityIntent describes the closeness
						state = active;
						(*totalActiveNodes)++;
					}
				}
				break;

			case totalActive:
				if (*totalActiveNodes >= activityIntent) {				// AI is necessary total number of active nodes
					if (state != active) {
						state = active;
						(*totalActiveNodes)++;
					}
				}
				break;

			case amountOfExposure:
				if (exposurePressure >= activityIntent) {				// AI is the necessary number of exposures
					if (state != active) {
						state = active;
						(*totalActiveNodes)++;
					}
					exposurePressure = 0;
				}
				else exposurePressure++;
				break;

			case influenceBased:
				if (other->influence >= activityIntent) {				// AI is necessary influence level of other node
					if (state != active) {
						state = active;
						(*totalActiveNodes)++;
					}
				}
				break;
			}
		}
	}

	void addPendingInteraction(Node* other) {
		pendingInteractions.push_back(other);
	}

	void goThroughInteractions(int* totalActiveNodes, int* totalAwareNodes) {
		if (pendingInteractions.size() == 0) {
			if (state == aware) {
				awarenessLevel -= awarenessDecayRate;
				if (awarenessLevel <= 0.0f) {
					state = ignorant;
					awarenessLevel = 0.0f;
				}
			}
			exposurePressure = std::max(0.0f, exposurePressure - exposureDecayRate);
		}	
		else
			for (int i = 0; i < pendingInteractions.size(); ++i) {
				interact(pendingInteractions[i], totalActiveNodes, totalAwareNodes);
			}
		pendingInteractions.clear();
	}

	bool enoughEnergy() {
		if (energy - energyCost <= 0)
			return false;
		else {
			energy -= energyCost;
			return true;
		}
	}
};

#endif // STRUCTS_H
