import numpy as np
import pandas as pd
import plotly.express as px

import plotly.graph_objects as go
from datetime import datetime, timedelta


def ema(data, span, column):
    alfa = 2/(span+1)
    ema_values = []
    for i, row in data.iterrows():
        denominator = 1
        p0 = row[column]
        numerator = p0
        if column == 'MACD' and i < span:
            continue
        if i < 26 and column != 'MACD':
            continue
        for k in range(span):
            pi = data[column].iloc[i-k-1]
            numerator += pi * ((1-alfa)**(k+1))
            denominator += (1-alfa)**(k+1)
        ema_values.append(numerator/denominator)
    return ema_values


def create_intersections(df1, df2, column1, column2):
    # return tuple with pandas.Dataframes containing buy and sell intersections
    intersectionsBuy = []
    intersectionsDatesBuy = []
    intersectionsSell = []
    intersectionsDatesSell = []
    for i in range(len(df2)):
        if df2[column2].iloc[i - 1] > df1[column1].iloc[i - 1] and df2[column2].iloc[i] < df1[column1].iloc[i]:
            intersectionsBuy.append((df1[column1].iloc[i]))
            intersectionsDatesBuy.append(df1['Date'].iloc[i])
        elif df2[column2].iloc[i - 1] < df1[column1].iloc[i - 1] and df2[column2].iloc[i] > df1[column1].iloc[i]:
            intersectionsSell.append((df1[column1].iloc[i]))
            intersectionsDatesSell.append(df1['Date'].iloc[i])
    return (pd.DataFrame({'Intersections': intersectionsBuy,
                            'Date': intersectionsDatesBuy}),
            pd.DataFrame({'Intersections': intersectionsSell,
                            'Date': intersectionsDatesSell}))


def buySell(data, intersectionsDf, capital, threshold, noThreshold):
    currentShares = 0
    ignoreFirst = False
    if intersectionsDf[0]['Date'].iloc[0] > intersectionsDf[1]['Date'].iloc[0]:
        ignoreFirst = True
    for i, row in intersectionsDf[0].iterrows():
            currentValueRow = data.loc[data['Date'] == row['Date']]
            currentValue = currentValueRow['Open'].iloc[0]
            if row['Intersections'] < -threshold or noThreshold:
                while capital - currentValue > 0:
                    capital -= currentValue
                    currentShares += 1
                if ignoreFirst:
                    if i + 1 < len(intersectionsDf[1]):
                        if intersectionsDf[1]['Intersections'].iloc[i + 1] > threshold or noThreshold:
                            while currentShares != 0:
                                currentValueRow = data.loc[data['Date'] == intersectionsDf[1]['Date'].iloc[i + 1]]
                                currentValue = currentValueRow['Open'].iloc[0]
                                capital += currentValue
                                currentShares -= 1
                else:
                    if intersectionsDf[1]['Intersections'].iloc[i] > threshold or noThreshold:
                        while currentShares != 0:
                            currentValueRow = data.loc[data['Date'] == intersectionsDf[1]['Date'].iloc[i]]
                            currentValue = currentValueRow['Open'].iloc[0]
                            capital += currentValue
                            currentShares -= 1
    if currentShares:
        currentValue = data['Open'].iloc[len(data)-1]
        while currentShares:
            capital += currentValue
            currentShares -= 1
    return capital


def createMACD(data):
    ema12 = ema(data, 12, 'Open')
    ema26 = ema(data, 26, 'Open')

    macd = []
    for i in range(len(ema12)):
        macd.append(ema12[i]-ema26[i])

    macdDf = pd.DataFrame({'MACD': macd,
                           'Date': data["Date"].iloc[26:]})
    macdDf.index = np.arange(0, len(macdDf))
    signal = ema(macdDf, 9, 'MACD')
    signalDf = pd.DataFrame({'Signal': signal,
                             'Date': data["Date"].iloc[35:]})
    macdDf = pd.DataFrame({'MACD': macd[9:], # cutting off 9 values to align with signal
                           'Date': data["Date"].iloc[35:]})
    macdDf.index = np.arange(0, len(macdDf))
    intersectionsDf = create_intersections(macdDf, signalDf, 'MACD', 'Signal')
    return macdDf, signalDf, intersectionsDf


def createGraphs(data, name, macdDf, signalDf, intersectionsDf):
    fig = go.Figure()
    fig.add_trace(go.Scatter(name="MACD", x=macdDf["Date"], y=macdDf["MACD"]))
    fig.add_trace(go.Scatter(name="Signal", x=signalDf["Date"], y=signalDf["Signal"]))
    fig.add_trace(go.Scatter(name="Buy intersections", x=intersectionsDf[0]["Date"],
                             y=intersectionsDf[0]['Intersections'], mode='markers',
                             marker_color='green'))
    fig.add_trace(go.Scatter(name="Sell intersections", x=intersectionsDf[1]["Date"],
                             y=intersectionsDf[1]['Intersections'], mode='markers',
                             marker_color='red'))
    fig.add_trace(go.Scatter(name="Share value", x=data["Date"], y=data["Open"]))
    fig.update_layout(title=name,
                      xaxis_title="Date",
                      yaxis_title="MACD", )
    fig.data[0].line.color = "#ffe476"
    fig.data[1].line.color = "blue"
    fig.show()
    start_date = "2020-01-01"
    end_date = "2020-06-01"
    fig.update_xaxes(type="date", range=[start_date, end_date])
    fig.show()
    data = pd.read_csv('KO.CSV')
    #valueFig = px.line(data, x="Date", y="Open", title=name + " Share Price")
    #valueFig.show()

dataKO = pd.read_csv("KO.csv")
dataCHWY = pd.read_csv("CHWY.csv")
dataZM = pd.read_csv("ZM.csv")

macdKO = createMACD(dataKO)
macdCHWY = createMACD(dataCHWY)
macdZM = createMACD(dataZM)
createGraphs(dataKO, "Coca-Cola Company", macdKO[0], macdKO[1], macdKO[2])