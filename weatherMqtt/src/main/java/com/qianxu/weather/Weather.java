package com.qianxu.weather;


import com.qianxu.dto.SolarMsg;
import com.qianxu.dto.WeatherMsg;

public interface Weather{
    public void init();
    public void init(String location);
    public WeatherMsg getWeather(String location);
    public WeatherMsg getWeather();
    //维度 精度 海拔
    public SolarMsg getSolar(Double lat,Double lon,Integer alt);
}