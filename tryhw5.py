from uwimg import *
from tqdm import tqdm
import pandas as pd

def softmax_model(inputs, outputs):
    l = [make_layer(inputs, outputs, SOFTMAX)]
    return make_model(l)

def neural_net(inputs, outputs, activation=LOGISTIC):
    print(inputs)
    l = [   make_layer(inputs, 32, activation),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

def deep_net(inputs, outputs):
    print(inputs)
    l = [   make_layer(inputs, 64, RELU),
            make_layer(64, 32, RELU),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

if __name__ == "__main__":
    print("loading data...")
    train = load_classification_data(b"cifar.train", b"cifar.labels", 1)
    test  = load_classification_data(b"cifar.test",  b"cifar.labels", 1)
    print("done")
    print

    print("training model...")
    batch = 128
    # iters = 1000
    iters = 1000
    rates = [1e0, 1e-1, 1e-2, 1e-3]
    # rate = 1e-1
    momentum = .9
    decay = .0
    # decay_list = [1e0, 1e-1, 1e-2, 1e-3, 1e-4]
    activation = LOGISTIC
    # activation_list = [LOGISTIC, RELU, LRELU]
    results = []

    for rate in tqdm(rates):
        iter_res = {"lr":rate}
        m = neural_net(train.X.cols, train.y.cols, activation=activation)
        train_model(m, train, batch, iters, rate, momentum, decay)
        print("done")
        print

        print("evaluating model...")
        iter_res["train_acc"] = accuracy_model(m, train)
        iter_res["test_acc"] = accuracy_model(m, test)
        print(iter_res)
        results.append(iter_res)
    
    pd.DataFrame(results).to_csv("q4_2.csv")

