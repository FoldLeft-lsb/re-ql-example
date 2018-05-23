let str = ReasonReact.string;

open Types;

module ModuleQuery = [%graphql {|
  {
   hello
 }
|}];

let moduleQuery = ModuleQuery.make();

let makeConnection = () : ApolloClient.generatedApolloClient =>
  ReasonApollo.createApolloClient(
    ~link=
      ApolloLinks.createHttpLink(
        ~uri="https://q80vw8qjp.lp.gql.zone/graphql",
        (),
      ),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

type state_t = {client: ApolloClient.generatedApolloClient};

type action_t =
  | NoUpdate;

let component = ReasonReact.reducerComponent("ReqlTest");

let make = _children => {
  ...component,
  initialState: () => {client: makeConnection()},
  didMount: self =>
    Reql.query(self.state.client, Reql.prepareQuery(moduleQuery))
    |> Js.Promise.then_(res => {
         Js.log("Re didMount: ");
         Js.log(res##data);
         Js.Promise.resolve();
       })
    |> ignore,
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};