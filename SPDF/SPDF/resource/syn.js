import {syn_SPOL} from "./__story__/syn_SPOL.js"
export function main(){
    SPDF.loadStory(syn_SPOL.PartA)
    SPDF.loadStory(syn_SPOL.PartB)
    // var master_love_index = SPDF.branch(["选项一","选项二","选项三"])
    // if (master_love_index == 0){
    //     SPDF.loadSPOL(syn_SPOL.PartB)
    // }else{
    //     SPDF.loadSPOL(syn_SPOL.PartC)
    // }
    // master_love_index += SPDF.branch(["选项一","选项二","选项三"])
    // if (master_love_index>5){
    //     SPDF.loadSPOL(syn_SPOL.PartGoodEnd)
    // }else{
    //     SPDF.loadSPOL(syn_SPOL.PartBadEnd)
    // }
}

/*SPOL0.10.0
-->PartA
>>>青雅音/2/3//1:???:3333>>>T2:ssss
SWITCH 2
    CASE 1
###
    sss
###
        >>>:"Hello World 1"
    CASE 2
        >>>:"Hello World 2#3333" #333333
        SWITCH 5
            CASE 4
                >>>:"Hello World 2-4"
            CASE 5
                >>>:"Hello World 2-5"
                SWITCH 1
                    CASE 1
                        >>>:"Hello World 2-5-1"
                    CASE 2
                        >>>:"Hello World 2-5-2"
    CASE 3
        >>>:"Hello World 3"
>>>:"Hello World 0"

-->PartB
>>>罗德岛干员//1:后勤部干员A:昨天的月末后勤满意度调查情况如何？>>>罗德岛干员:
>>>罗德岛干员//1:>>>罗德岛干员:后勤部干员B:总的来说还不错，但是......但是博士一直在跟我们抱怨这个月的空调系统是不是有毛病
>>>罗德岛干员//1:后勤部干员A:空调系统？空调系统可是上个月月末刚刚彻底检修过一遍的。>>>罗德岛干员:
>>>罗德岛干员//1:后勤部干员A:博士房间的门在罗德岛行进过程中自己颠开的概率都比空调系统出问题的概率大吧？
>>>可露希尔:我听说有人在抱怨空调系统？
>>>罗德岛干员//1:后勤部干员A:啊，是可总师！
>>>可露希尔:所以到底是什么情况？
>>>罗德岛干员//1:后勤部干员B:博士说过去一个月空调系统的情况都不能让他满意，我们暂不清楚是什么情况，需要我们去调查一下么？
>>>可露希尔:博士？
SPOL*/
    
