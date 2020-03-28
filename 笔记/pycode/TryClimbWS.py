import requests
url="https://fanyi.baidu.com/#en/zh/Hello%20World"
resp=requests.get(url)
file=open("a.txt",'w',encoding='utf-8')

url = "https://fanyi.baidu.com/v2transapi"
data = {
        "from": "en",
        "to": "zh",
        "query": "hello",
        "transtype": "translang",
        "simple_means_flag": "3",
        "token": "f8c8ea6c95d6e9e7ba318e136ee2c490",
        "sign": "54706.276099" # 随着翻译内容变化而变化
        }

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36",
    "Cookie": "BDUSS=I2N2tPRVBXZk5zQnZJdmV6QnY5ZDc1anNEU2lrcWVCN25lQ2t6bXJ2amd5a1JiQVFBQUFBJCQAAAAAAAAAAAEAAACiHXkweGllY2hlbmcxOTk1AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOA9HVvgPR1bcT; BIDUPSID=FCDC0DE1E2A0BC542BE845EF57F0DA5F; PSTM=1560475969; BAIDUID=FCDC0DE1E2A0BC54F756173A5902B1FB:FG=1; locale=zh; REALTIME_TRANS_SWITCH=1; FANYI_WORD_SWITCH=1; HISTORY_SWITCH=1; SOUND_SPD_SWITCH=1; SOUND_PREFER_SWITCH=1; to_lang_often=%5B%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%2C%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%5D; from_lang_often=%5B%7B%22value%22%3A%22en%22%2C%22text%22%3A%22%u82F1%u8BED%22%7D%2C%7B%22value%22%3A%22zh%22%2C%22text%22%3A%22%u4E2D%u6587%22%7D%5D; Hm_lvt_afd111fa62852d1f37001d1f980b6800=1561467752,1561468730,1561469040,1561469241; Hm_lpvt_afd111fa62852d1f37001d1f980b6800=1561469241; Hm_lvt_64ecd82404c51e03dc91cb9e8c025574=1561468730,1561469040,1561469241,1561470454; Hm_lpvt_64ecd82404c51e03dc91cb9e8c025574=1561470454; yjs_js_security_passport=5453d43730575af39e7a665570b61ece7f5fc95b_1561470456_js"
}

response = requests.post(url, data=data, headers=headers)

print(response)
print(response.content.decode('unicode_escape')) # 中文转码

#file.write(resp.text)
file.write(response.content.decode('unicode_escape'))
file.close()
