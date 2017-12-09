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

    // loop forever running episodes of lunar lander
    int count = 0;

    float episodeReward = 0;
    std::vector<float> episodeRewards;
    float meanReward = 0;
    bool log = 0;
    while (1) {
        count++;

        EnvData envData = env.reset();
        std::vector<float> observation;
        observation = envData.observation;
        log = false;

        // game should only last 1000 trials
        for (int i = 0; i < 1001; i++) {

            //if (count > 300) {
            //    env.render();
            //    std::this_thread::sleep_for(std::chrono::milliseconds(10));
            //}
            //if (count > 100) {
            //    if (count % 10 == 0) {
            //        env.render();
            //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
            //    }
            //}
            std::vector<float> action; action.clear();
            int iact = reinforceAgent.selectAction(observation);
            action.push_back(iact);
            envData = env.step(action);
            observation = envData.observation;

            if (log) {
                env.render();
                std::this_thread::sleep_for(std::chrono::milliseconds(300));

                std::cout << "action: " << iact << std::endl;

                std::cout << "obs: ";
                for (std::vector<float>::const_iterator i = envData.observation.begin(); i != envData.observation.end(); ++i)
                    std::cout << *i << ' ';
                std::cout << std::endl;

                std::cout << "reward: " << envData.reward << endl;
                if (envData.done) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                }
            }


            episodeReward += envData.reward;
            episodeRewards.push_back(envData.reward);
            //std::cout << "action: " << action.front() << " reward: " << envData.reward << std::endl;
            if (envData.done) {
                break;
            }
            assert(i < 1000);
        }

        // lander has crashed or landed successfully or gone off screen
        meanReward += episodeReward;
        episodeReward = 0;
        //reinforceAgent.optimizePolicy(episodeRewards, true);

        if (count % 10 == 0) {
            reinforceAgent.optimizePolicy(episodeRewards, true);
        }
        else {
            reinforceAgent.optimizePolicy(episodeRewards, false);
        }
        episodeRewards.clear();


        if (count % 100 == 0) {
            std::cout << "done, ep reward = " << meanReward/100 << std::endl;
            //if (meanReward / 100 <= -400) {
            //    log = true;
            //}
            meanReward = 0;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}
