const express=require("express")
const app=express()
const port = 3000; 

app.get("/req",(req,res)=>{
    console.log("request is comming");
    let massege={
        "name": "sakib",
        "age": "30"
        }
    res.status(200).json(massege)
    // res.json(massege)
})

app.listen(port,console.log(`server is listning on port ${port}`))