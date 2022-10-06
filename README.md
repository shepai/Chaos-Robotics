# Chaos-Robotics
Repository for robot development using chaotic dynamics. This is part of the work for my doctoral thesis. Here we will explore different ways of using chaotic dynamics for motor control, with the end goal of a resiliant robot that adapts to terrain.

## Introduction
Robust motor control for locomotion of robots with a relatively high number of degrees-of-freedom in complex
environments is a challenging problem. An important part of this challenge is resilience - the ability to adapt in real time to damage or to significant changes in environmental conditions (e.g flat to uneven to slippy ground). Biologically inspired approaches are among the most promising in this area, particularly with respect to control architectures and sensory feedback.
TacTips and neuromorphic (event) cameras are candidates for such sensing. Using a sense of touch from
skin-inspired technology enables research into embodied interactions with an environment without focusing
solely on expensive computer vision processing. Plasticity within robotics is essential for robots deployed
within environments where humans cannot alter hardware if damage occurs. For example, Mars exploration
robots having parts breakdown while travelling over advanced terrain. Bio-inspired neural networks that
exploit chaotic dynamics for goal-oriented behaviour is a promising method for this resilience, particularly as it enables real-time adaptation and reconfiguration without the need to build and maintain an internal model of the robot and its interactions with environment. Although this method has been demonstrated in simulation it has not yet been properly explored on physical robots engaged in goal-directed behaviour.
This project aims to provide solutions to the problem of robust motor control via nature-inspired feedback
and control - specifically using TacTips and chaotic neural control. The project centres itself around resilience on physical robots. This will be shown through a complex chassis with multiple degrees of freedom traversing over uneven terrain.
Using TacTips on a physical robot with chaotic control has not been done before, as these projects are
typically evaluated in simulation and never get to the physical implementation stage. Physical robotics is
much harder due to real-world noise interference, which is why the TacTip sensing offers better-embodied
modelling. TacTips offer texture dimension thus richer textural information, which will be perfect for plasticity and reconfiguration of gaits. This has not been done on physical robots in the past. Resilient physical robots have typically used contact switches to determine where the ground is [1].
Resilience is an emerging area in the field of robotics for the reasons of making more robust robots
for advanced terrain, as well as potential applications for robotic prosthetic limbs [2] [3]. Many of the
current problems within resilience are about adaptation relationships, such as the relation between failure
and recovery. This is in addition to having better hardware relationships to interface with an environment
with reliability [4].

## References
[1] William A Lewinger and Roger D Quinn. Neurobiologically-based control system for an adaptively
walking hexapod. Industrial Robot: An International Journal, 2011.
[2] Darko Trivun, Haris Dindo, and Bakir Laˇcevi ́c. Resilient hexapod robot. In 2017 XXVI International
Conference on Information, Communication and Automation Technologies (ICAT), pages 1–6, 2017.
[3] Pengju Li, Hashina Parveen Anwar Ali, Wen Cheng, Jingyi Yang, and Benjamin CK Tee. Bioinspired
prosthetic interfaces. Advanced Materials Technologies, 5(3):1900856, 2020.
[4] Tan Zhang, Wenjun Zhang, and Madan M Gupta. Resilient robots: concept, review, and future direc-
tions. Robotics, 6(4):22, 2017.
[5] R.T. Schroer, M.J. Boggess, R.J. Bachmann, R.D. Quinn, and R.E. Ritzmann. Comparing cockroach
and whegs robot body motions. In IEEE International Conference on Robotics and Automation, 2004.
Proceedings. ICRA ’04. 2004, volume 4, pages 3288–3293 Vol.4, 2004.
[6] Guanjiao Ren, Weihai Chen, Christoph Kolodziejski, Florentin W ̈org ̈otter, Sakyasingha Dasgupta, and
Poramate Manoonpong. Multiple chaotic central pattern generators for locomotion generation and leg
damage compensation in a hexapod robot. In 2012 IEEE/RSJ International Conference on Intelligent
Robots and Systems, pages 2756–2761. IEEE, 2012.
[7] Kohei Nakajima, Helmut Hauser, Rongjie Kang, Emanuele Guglielmino, Darwin G Caldwell, and Rolf
Pfeifer. A soft body as a reservoir: case studies in a dynamic model of octopus-inspired soft robotic
arm. Frontiers in computational neuroscience, 7:91, 2013.
[8] Gary Parker, D Braun, and Ingo Cyliax. Evolving hexapod gaits using a cyclic genetic algorithm.
In Proceedings of the IASTED International Conference on Artificial Intelligence and Soft Computing
(ASC’97), pages 141–144. Citeseer, 1997.
[9] Yoonsik Shim and Phil Husbands. Embodied neuromechanical chaos through homeostatic regulation.
Chaos: An Interdisciplinary Journal of Nonlinear Science, 29(3):033123, 2019.
[10] K. Aihara, T. Takabe, and M. Toyoda. Chaotic neural networks. Physics Letters A, 144(6):333–340,
1990.
[11] Benjamin Ward-Cherrier, Nicolas Rojas, and Nathan F. Lepora. Model-free precise in-hand manipulation
with a 3d-printed tactile gripper. IEEE Robotics and Automation Letters, 2(4):2056–2063, 2017.
[12] Elisabeth Ann Stone, Nathan F. Lepora, and David A. W. Barton. Walking on tactip toes: A tactile
sensing foot for walking robots. 2020 IEEE/RSJ International Conference on Intelligent Robots and
Systems (IROS), pages 9869–9875, 2020.
[13] Xiangxiao Liu, Andre Rosendo, Shuhei Ikemoto, Masahiro Shimizu, and Koh Hosoda. Robotic inves-
tigation on effect of stretch reflex and crossed inhibitory response on bipedal hopping. Journal of The
Royal Society Interface, 15(140):20180024, 2018.
[14] Guang Chen, Hu Cao, Jorg Conradt, Huajin Tang, Florian Rohrbein, and Alois Knoll. Event-based neu-
romorphic vision for autonomous driving: a paradigm shift for bio-inspired visual sensing and perception.
IEEE Signal Processing Magazine, 37(4):34–49, 2020.
6
