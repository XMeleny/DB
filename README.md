# DB 

为了只用一个远程仓库，使用方法如下：

首先选择你最喜欢的路径

//这一句我不知道有没有用，如果大家看到git remote -v不是连接XMeleny的话就用这一句话
git remote add origin https://github.com/XMeleny/DB

git clone https://github.com/XMeleny/DB

//do you job

git add .

git commit -m"."

git push origin develop//注意是develop！不是master！

大家提交的都在github的Dev上面，我每天或者有空就merge dev到master。
开工干活就pull master，
完成了一个小功能就commit一下，
一天不想干了就push上dev去。
