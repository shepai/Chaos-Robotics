import sys
import gym
import gym
from gym import logger as gymlogger
import matplotlib.pyplot as plt
from IPython import display as ipythondisplay
from pyvirtualdisplay import Display
import random
env = gym.make("Acrobot-v1",render_mode="rgb_array")
observation, info = env.reset()
show=False
maxReward=0
for _ in range(1000):
    action = env.action_space.sample()
    action=random.randint(0,2)
    observation, reward, terminated, truncated, info = env.step(action)
    rendered=env.render()
    plt.imshow(rendered)
    plt.pause(0.05)
    if terminated or truncated:
        observation, info = env.reset()
env.close()

print("done")