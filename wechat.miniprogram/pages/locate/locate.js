const devicesId = "577850820" // 填写在OneNet上获得的devicesId 形式就是一串数字
const api_key = "9LAYchwxyELmH6WbTgpXQYL7Jz4=" // 填写在OneNet上的 api-key
Page({
  data: {
    lati: " ",
    longi:" ",
    markers: [{
      id: '0',
      latitude: " ",
      longitude: " ",
      width: 50,
      height: 50
    }]
  },
//向OneNet请求当前设备的数据点
  getDatapoints: function () {
    var that = this
    wx.request({
      url: `https://api.heclouds.com/devices/577850820/datapoints?datastream_id=latitude,longitude`,
      header: {
        'content-type': 'application/json',
        'api-key': api_key
      },
      success: function (res) {
        console.log(res.data.data),
          that.setData({
            "lati": res.data.data.datastreams[0].datapoints[0].value,//将Onenet的值附到data中的数据里，获得纬度
            "longi": res.data.data.datastreams[1].datapoints[0].value,//获得经度
            "markers": [{
              id: '0',
              latitude: "  res.data.data.datastreams[0].datapoints[0].value",
              longitude: " res.data.data.datastreams[1].datapoints[0].value",
              width: 50,
              height: 50
            }]//试图重新初始化markers，但是没有起作用
          })
      }
    })
    console.log(that.data.lati)//验证纬度是否传递成功
    console.log(that.data.longi)//验证经度是否传递成功
  }
})