import gym
import gym_kraby

env = gym.make('gym_kraby:HexapodBulletEnv-v0', render=True)
observation = env.reset()
score_return = 0
done = False

while not done:
    a = env.action_space.sample()  # take a random action
    observation, reward, done, _ = env.step(a)  # step
    score_return += reward

print("Environment episode is done, your total return was", score_return)
env.close()