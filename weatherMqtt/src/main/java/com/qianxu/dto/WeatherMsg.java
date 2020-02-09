package com.qianxu.dto;

public class WeatherMsg {

    private Integer pm10;
    private Integer pm25;
    //降雨
    private Double pcpn;

    private String location;

    public Integer getPm10() {
        return pm10;
    }

    public void setPm10(Integer pm10) {
        this.pm10 = pm10;
    }

    public Integer getPm25() {
        return pm25;
    }

    public void setPm25(Integer pm25) {
        this.pm25 = pm25;
    }

    public Double getPcpn() {
        return pcpn;
    }

    public void setPcpn(Double pcpn) {
        this.pcpn = pcpn;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }


    @Override
    public String toString() {
        return "WeatherMsg{" +
                "pm10=" + pm10 +
                ", pm25=" + pm25 +
                ", pcpn=" + pcpn +
                ", location='" + location + '\'' +
                '}';
    }
}