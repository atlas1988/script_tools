# script_tools
python ----python code &tools
shell  ----shell code & tools
bat    ----bat code & tools


运行脚本
ssh-key.sh
ssh-keygen -t rsa
sudo cp ~/.ssh/id_rsa.pub id_rsa.pub
vi id_rsa.pub
一直回车，生成sshkey
将sshkey 设置到对应的帐号
打开githhu 个人设置
settings-->SSH and GPG keys ->NEW SSH key --> copy sshkey to key and set the title

进入目录 clone 代码
git clone git@github.com:atlas1988/script_tools.git
设置好对应的git config user.name / user.email(可以直接运行git-config-atlas.sh)

添加改动
git add
git commit -m
git push


content
shell/android_env_setup --android开发环境设置（编译环境 repo vim 远程连接 adb 设置）
