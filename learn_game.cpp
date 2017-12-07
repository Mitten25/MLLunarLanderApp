#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include "envs/env.h"
#include "envs/lunarlander.h"
#include "rl/reinforce.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int learn_game()
{
    LunarLander env(false);
    // discrete
    int observationSpace = 8;
    int actionCount = 4;

    REINFORCE reinforceAgent(observationSpace, actionCount);

    EnvData envData = env.reset();

    float episodeReward = 0;
    std::vector<float> episodeRewards;
    float meanReward = 0;

    // loop forever running episodes of lunar lander
    int count = 0;
    // take first observation
    std::vector<float> observation;
    std::vector<float> s(1, 0.0f);
    envData = env.step(s);
    observation = envData.observation;

    while (1) {
        count++;

        //env.render();
        std::vector<float> action;
        action.clear();
        action.push_back(reinforceAgent.selectAction(observation));
        envData = env.step(action);
        observation = envData.observation;
        episodeReward += envData.reward;
        episodeRewards.push_back(envData.reward);
        //std::cout << "action: " << action.front() << " reward: " << envData.reward << std::endl;



        // lander has crashed or landed successfully or gone off screen
        if (envData.done) {
            envData = env.reset();
            meanReward += episodeReward;
            episodeReward = 0;
            reinforceAgent.optimizePolicy(episodeRewards, true);
            episodeRewards.clear();

            if (count % 100 == 0) {
                std::cout << "done, ep reward = " << meanReward/100 << std::endl;
                meanReward = 0;
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
